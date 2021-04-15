#include "../../include/Server/TTRClient.h"
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
namespace ttr {

TTRServer::TTRServer(TTRController *c) {
    stub_ = TTRService::NewStub(grpc::CreateChannel(
        "localhost:50051", grpc::InsecureChannelCredentials()));
    controller = c;
}
BoardState TTRServer::get_board_state() {
    BoardState state;
    Board board;
    Deck deck;
    std::vector<::Path> all_paths = controller->get_all_paths();
    for(int i = 0; i < all_paths.size(); i++){
        const ::Path& path = all_paths[i];
        ttr::Path n_path;
        n_path.set_color(path.color);
        n_path.set_finish(path.finish);
        n_path.set_start(path.start);
        n_path.set_number_of_colored_wagons(path.number_of_colored_wagons);
        n_path.set_number_of_locomotives(path.number_of_locomotives);
        n_path.set_is_tunnel(path.is_tunnel);
        n_path.set_owner(path.owner);
        board.add_paths()[i] = n_path;
    }

    std::vector<WagonCard> active_cards = controller->get_active_cards();
    for(int i = 0; i < active_cards.size(); i++){
        Card n_card;
        n_card.set_type(::ttr::Card_Type_Wagon);
        Wagon w;
        w.set_color(active_cards[i].color);
        *n_card.release_wagon_info() = w;
        deck.add_cards_on_table()[i] = n_card;
    }
    *state.release_deck_state() = deck;
    *state.release_board_state() = board;

    return state;
}
MakeTurnResponse TTRServer::make_turn_request(const MakeTurnRequest& request) {
    MakeTurnResponse response;
    response.set_is_success(true);
    if(request.type() == "get board state"){
        *response.release_current_state() = get_board_state();
        return response;
    }
    if(request.id().id() != controller->get_current_player_id()) {
        response.set_is_success(false);
        return response;
    }

    if (request.type() == "draw from deck"){ //TODO make global constants
        controller->get_card_from_deck();
    }
    if(request.type() == "draw from active"){
        controller->get_card_from_active(request.active_card_id());
    }
    if(request.type() == "build path"){
        controller->build_path_initialize(request.path_to_build_id());
    }
    if(request.type() == "build station"){
        //TODO
    }
    return response;
}
}  // namespace ttr