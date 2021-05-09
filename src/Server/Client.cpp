#include "Server/Client.h"
namespace{
    Rectangle parse_grpc_rectangle(const ::ttr::Rectangle& r){
        Rectangle ans;
        for(int i = 0; i < 4; i++){
            const ::ttr::Point& current_point = r.points(i);
            ans.points.push_back({current_point.x(), current_point.y()});
        }
        return ans;
    }
}
GameClient::GameClient() {
    stub_ = ::ttr::TTRService::NewStub(grpc::CreateChannel(
        "localhost:50051", grpc::InsecureChannelCredentials()));

}
ttr::BoardState *GameClient::get_board_state() {
    auto *state = new ttr::BoardState();
    ttr::Nothing request;
    auto *client_context = new ::grpc::ClientContext();
    stub_->get_board_state(client_context, request, state);
    return state;
}
std::vector<Path> GameClient::get_paths() {
    auto state = get_board_state();
    auto board = *(state->release_board_state());
    std::vector<Path> paths;
    for (int i = 0; i < board.paths_size(); i++) {
        const auto &path = board.paths(i);
        Path new_path;
        new_path.color = path.color();
        new_path.number_of_colored_wagons = path.number_of_colored_wagons();
        new_path.number_of_locomotives = path.number_of_locomotives();
        new_path.owner = path.owner();
        new_path.is_tunnel = path.is_tunnel();
        new_path.start = path.start();
        new_path.finish = path.finish();
        new_path.length = path.length();
        for(int j = 0; j < path.wagon_blocks_size();j++){
            WagonBlock n_block;
            const auto& wagon_to_parse = path.wagon_blocks(j);
            n_block.coords = parse_grpc_rectangle(wagon_to_parse.coords());
            n_block.color = wagon_to_parse.color();
            n_block.id = wagon_to_parse.id();
            new_path.wagon_blocks.push_back(n_block);
        }
        paths.push_back(new_path);
    }
    return paths;
}
int GameClient::get_id() {
    auto* id = new ::ttr::PlayerID;
    ttr::Nothing request;
    auto *client_context = new ::grpc::ClientContext();
    stub_->get_player_id(client_context, request, id);
    return id->id();
}
void GameClient::start_game() {
    ::ttr::Nothing req;
    stub_->start_game(nullptr, req, nullptr);
}
std::vector<WagonCard> GameClient::get_active_cards() {
    auto state = get_board_state();
    auto deck = state->release_deck_state();
    std::vector<WagonCard> active_cards;
    for (int i = 0; i < deck->cards_on_table_size(); i++) {
        WagonCard n_card;
        n_card.color = deck->cards_on_table(i).wagon_info().color();
        active_cards.push_back(n_card);
    }
    return active_cards;
}
std::vector<WagonCard> GameClient::get_player_cards(int id) {
    auto player_state = get_state(id);
    std::vector<WagonCard> cards;
    for(int i = 0; i < player_state.mutable_private_info()->mutable_player_wagons()->wagons_size();i++){
        auto wagon = player_state.mutable_private_info()->mutable_player_wagons()->wagons(i);
        WagonCard n_card(wagon.color());
        cards.push_back(n_card);
    }
    return cards;
}
ttr::PlayerState GameClient::get_state(int id) {
    auto context = new ::grpc::ClientContext();
    ::ttr::PlayerID player_id;
    player_id.set_id(id);
    auto state = new ttr::PlayerState();
    stub_->get_player_state(context, player_id, state);
    return *state;
}
std::vector<Player> GameClient::get_all_players() {
    auto board_state = get_board_state();
    std::vector<Player> player_general_info(board_state->all_players().all_players_size());
    for(int i = 0; i < board_state->all_players().all_players_size();i++){
        auto curr_player = board_state->all_players().all_players(i);
        player_general_info[i].points = curr_player.current_score();
        player_general_info[i].number_of_wagons_left = curr_player.wagons_left();
        auto private_info = get_state(i);//tssss it's a secret
        player_general_info[i].wagon_cards.resize(private_info.release_private_info()->release_player_wagons()->wagons_size());
        player_general_info[i].number_of_stations_left = 5;//TODO redo
    }
    return player_general_info;
}
