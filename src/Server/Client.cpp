#include "Server/Client.h"
GameClient::GameClient() {
    stub_ = ::ttr::TTRService::NewStub(grpc::CreateChannel(
        "localhost:50051", grpc::InsecureChannelCredentials()));
}
ttr::BoardState *GameClient::get_board_state() {
    auto *state = new ttr::BoardState();
    ttr::Nothing request;
    auto *client_context = new ::grpc::ClientContext();
    stub_->get_board_state(client_context, request, state);
    std::cout << "oh my god";
    return state;
}
