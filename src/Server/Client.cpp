#include "Server/Client.h"
#include <vector>
#include <utility>
namespace {
Rectangle parse_grpc_rectangle(const ::ttr::Rectangle &r) {
    Rectangle ans;
    for (int i = 0; i < 4; i++) {
        const ::ttr::Point &current_point = r.points(i);
        ans.points.push_back({current_point.x(), current_point.y()});
    }
    return ans;
}
}  // namespace

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
        for (int j = 0; j < path.wagon_blocks_size(); j++) {
            WagonBlock n_block;
            const auto &wagon_to_parse = path.wagon_blocks(j);
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
    auto *id = new ::ttr::PlayerID;
    ttr::Nothing request;
    auto *client_context = new ::grpc::ClientContext();
    stub_->get_player_id(client_context, request, id);
    return id->id();
}

void GameClient::start_game() {
    auto *req = new ::ttr::Nothing();
    auto *client_context = new ::grpc::ClientContext();
    auto *response = new ::ttr::Nothing();
    stub_->start_game(client_context, *req, response);
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
    for (int i = 0; i < player_state.mutable_private_info()
                            ->mutable_player_wagons()
                            ->wagons_size();
         i++) {
        auto wagon = player_state.mutable_private_info()
                         ->mutable_player_wagons()
                         ->wagons(i);
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
    std::vector<Player> player_general_info(
        board_state->all_players().all_players_size());
    for (int i = 0; i < board_state->all_players().all_players_size(); i++) {
        auto curr_player = board_state->all_players().all_players(i);
        player_general_info[i].points = curr_player.current_score();
        player_general_info[i].number_of_wagons_left =
            curr_player.wagons_left();
        auto private_info = get_state(i);  // tssss it's a secret
        player_general_info[i].wagon_cards.resize(
            private_info.release_private_info()
                ->release_player_wagons()
                ->wagons_size());
        player_general_info[i].number_of_stations_left = 5;  // TODO redo
    }
    return player_general_info;
}

std::vector<int> GameClient::get_score() {
    auto context = new ::grpc::ClientContext();
    ::ttr::Nothing request;
    auto *response = new ::ttr::INT_ARRAY();
    stub_->get_score(context, request, response);
    std::vector<int> points;
    points.reserve(response->values_size());
    for (int i = 0; i < response->values_size(); i++) {
        points.push_back(response->values(i));
    }
    return points;
}

void GameClient::make_turn(Turn *t, int id) {
    if (get_id() != id) {
        return;
    }
    auto *context = new ::grpc::ClientContext();
    auto *request = new ::ttr::MakeTurnRequest();
    request->mutable_id()->set_id(id);
    std::cout << "i want to make turn and my id is " << request->id().id()
              << '\n';
    if (auto *p = dynamic_cast<DrawCardFromDeck *>(t); p) {
        request->set_type("draw from deck");
    }
    if (auto *p = dynamic_cast<DrawCardFromActive *>(t); p) {
        request->set_type("draw from active");
        request->set_active_card_id(p->number);
    }
    if (auto *p = dynamic_cast<TakeRoutes *>(t); p) {
        // todo
    }
    if (auto *p = dynamic_cast<BuildStation *>(t); p) {
        request->set_type("build station");
        request->set_station_to_build_id(p->get_city());
    }
    if (auto *p = dynamic_cast<BuildPath *>(t); p) {
        request->set_type("build path");
        request->set_path_to_build_id(p->get_pos());
        ::ttr::Wagon w;

        w.set_color(p->getWagons()[0].color);
        *(request->mutable_color_to_build()) = w;
    }
    auto *response = new ::ttr::Nothing();
    stub_->make_turn(context, *request, response);
}

int GameClient::get_number_of_players() {
    std::cout << "number_of_players()\n";
    auto *context = new ::grpc::ClientContext();
    auto request = new ::ttr::Nothing();
    auto *response = new ttr::PlayerID();
    stub_->get_number_of_players(context, *request, response);
    return response->id();
}

std::vector<std::pair<std::string, Circle>> GameClient::get_stations() {
    auto board = get_board_state();
    std::cout<<"try to get stations\n";
    std::vector<std::pair<std::string, Circle>> stations;
    for(int i = 0; i < board->board_state().stations_size();i++){
        auto station = board->mutable_board_state()->stations(i);
        Point coords{station.coords().x(), station.coords().y()};
        Circle point{coords, 5};
        std::string city = *station.mutable_city();
        stations.push_back(std::make_pair(city, point));
    }
    std::cout<<"got stations\n";
    return stations;
}
