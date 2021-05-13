#include "Server/TTRServer.h"
#include "TTRController.h"
#include <grpc/grpc.h>
#include <grpcpp/channel.h>
#include <grpcpp/client_context.h>
#include <grpcpp/create_channel.h>
#include <grpcpp/security/credentials.h>
using grpc::Channel;
using grpc::ClientContext;
using grpc::ClientReader;
using grpc::ClientReaderWriter;
using grpc::ClientWriter;
using grpc::Status;
namespace {
    ttr::Rectangle parse_to_grpc_rectangle(const Rectangle& r){
        ttr::Rectangle ans;
        for(auto point: r.points){
            ttr::Point n_point;
            n_point.set_x(point.x);
            n_point.set_y(point.y);
            (*ans.add_points()) = n_point;
        }
        return ans;
    }
    ttr::Card parse_card_to_grpc(const WagonCard& active_card){
        ttr::Card n_card;
        n_card.set_type(::ttr::Card_Type_Wagon);
        n_card.set_allocated_wagon_info(new ::ttr::Wagon());
        n_card.mutable_wagon_info()->set_color(active_card.color);
        return n_card;
    }
    ttr::Route parse_to_grpc_route(const Route& route){
        ttr::Route n_route;
        n_route.set_begin(route.city1);
        n_route.set_end(route.city2);
        n_route.set_points(route.points_for_passing);
        return n_route;
    }
    ttr::WagonBlock parse_to_grpc_wagon_block(const WagonBlock& w){
        ttr::WagonBlock n_block;
        n_block.set_id(w.id);
        n_block.set_color(w.color);
        n_block.set_allocated_coords(new ttr::Rectangle());
        *(n_block.mutable_coords()) = parse_to_grpc_rectangle(w.coords);
        return n_block;
    }

ttr::Wagon parse_to_grpc_wagon(const WagonCard& w){
    ttr::Wagon n_card;
    n_card.set_color(w.color);
    return n_card;
}
}
namespace ttr {

TTRServer::TTRServer(TTRController *c) {
    controller = c;
}

Nothing TTRServer::local_make_turn(
    const ::ttr::MakeTurnRequest *request) {
    Nothing response;
    if (request->id().id() != controller->get_current_player_id()) {

        return response;
    }

    if (request->type() == "draw from deck") {  // TODO make global constants
        controller->get_card_from_deck();
    }
    if (request->type() == "draw from active") {
        controller->get_card_from_active(request->active_card_id());
    }
    if (request->type() == "build path") {
        controller->build_path_initialize(request->path_to_build_id());
    }
    if (request->type() == "build station") {
        // TODO
    }
    return response;
}

::grpc::Status TTRServer::get_board_state(::grpc::ServerContext *context,
                                          const ::ttr::Nothing *request,
                                          ::ttr::BoardState *response) {
    *response = local_get_board_state();
    return ::grpc::Status::OK;
}

BoardState TTRServer::local_get_board_state() {

    ::ttr::BoardState state;
    Board board;
    Deck deck;
    std::vector<::Path> all_paths = controller->get_all_paths();
    for (auto & path : all_paths) {
        ttr::Path n_path;
        n_path.set_color(path.color);
        n_path.set_finish(path.finish);
        n_path.set_start(path.start);
        n_path.set_number_of_colored_wagons(path.number_of_colored_wagons);
        n_path.set_number_of_locomotives(path.number_of_locomotives);
        n_path.set_is_tunnel(path.is_tunnel);
        n_path.set_owner(path.owner);
        for(const auto& w: path.wagon_blocks){
            *(n_path.add_wagon_blocks()) = parse_to_grpc_wagon_block(w);
        }
        n_path.set_length(path.length);
        *(board.add_paths()) = n_path;
    }

    std::vector<WagonCard> active_cards = controller->get_active_cards();
    for (auto & active_card : active_cards) {
        *(deck.add_cards_on_table()) = parse_card_to_grpc(active_card);
    }
    *state.mutable_board_state() = board;
    *state.mutable_deck_state() = deck;
    state.set_allocated_all_players(new Players());
    for (int i = 0; i < controller->get_players().size(); i++) {
        auto player = controller->get_players()[i];
        ttr::Player p;
        p.set_current_score(controller->get_results()[player.id]);
        p.set_routes_num(player.active_routes.size());
        p.set_wagons_left(player.number_of_wagons_left);
        *(state.mutable_all_players()->add_all_players()) = p;
    }
    return state;
}

::grpc::Status TTRServer::make_turn(::grpc::ServerContext *context,
                                    const ::ttr::MakeTurnRequest *request,
                                    ::ttr::Nothing *response) {
    *response = local_make_turn(request);
    return ::grpc::Status::OK;
}
::grpc::Status TTRServer::get_score(::grpc::ServerContext *context,
                                    const Nothing *request,
                                    ::ttr::INT_ARRAY *result) {
    for(auto i: controller->get_results()){
        result->add_values(i);
    }
    return grpc::Status::OK;
}
::grpc::Status TTRServer::get_player_id(::grpc::ServerContext *context,
                                        const ::ttr::Nothing *request,
                                        ::ttr::PlayerID *response) {
    static int last_id = 0;
    if(!controller->is_game_started()){
        response->set_id(last_id);
        last_id++;
    }else{
        response->set_id(controller->get_current_player_id());
    }
    return ::grpc::Status::OK;
}
::grpc::Status TTRServer::start_game(::grpc::ServerContext *context,
                                     const ::ttr::Nothing *request,
                                     ::ttr::Nothing *result) {
    controller->start_game_server();
    return ::grpc::Status::OK;
}
::grpc::Status TTRServer::get_player_state(::grpc::ServerContext *context,
                                           const ::ttr::PlayerID *request,
                                           ::ttr::PlayerState *result) {
    std::cout<<"try to got state\n";
    auto current_player = controller->get_players()[request->id()];
    result->mutable_player_general_info()->set_routes_num(current_player.active_routes.size());
    result->mutable_player_general_info()->set_wagons_left(current_player.number_of_wagons_left);
    result->mutable_player_general_info()->set_current_score(current_player.points);
    std::cout<<"set some info\n";
    for(const auto &route: current_player.active_routes){
        *(result->mutable_private_info()->mutable_player_routes()->add_routes()) = parse_to_grpc_route(route);
    }
    for(const auto& wagon: current_player.wagon_cards){
        *(result->mutable_private_info()->mutable_player_wagons()->add_wagons()) = parse_to_grpc_wagon(wagon);
    }
    std::cout<<"all got\n";
    return grpc::Status::OK;
}
::grpc::Status TTRServer::check_is_bot(::grpc::ServerContext *context,
                                       const ::ttr::PlayerID *request,
                                       ::ttr::BOOL *result) {
    result->set_value(controller->get_players()[request->id()].is_bot);
    return ::grpc::Status::OK;
}

LocalServer::LocalServer(TTRController *c) : service(TTRServer(c)) {
    builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
    builder.RegisterService(&service);
    server = builder.BuildAndStart();
}

void LocalServer::runServer() {
    server->Wait();
}

void LocalServer::terminate() {
    server->Shutdown();
}

void RunServer(LocalServer *serv, bool needRun) {
    if (needRun) {
        serv->runServer();
    }
}

}  // namespace ttr
