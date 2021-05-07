#pragma once
#include <grpc/grpc.h>
#include <grpcpp/security/server_credentials.h>
#include <grpcpp/server.h>
#include <grpcpp/server_builder.h>
#include <grpcpp/server_context.h>
#include "TTR.grpc.pb.h"
#include "TTR.pb.h"
#include "TTRController_fwd.h"
namespace ttr {
class TTRServer : public ttr::TTRService::Service {
public:
    explicit TTRServer(TTRController *c);

    ::grpc::Status get_board_state(::grpc::ServerContext *context,
                                   const ::ttr::Nothing *request,
                                   ::ttr::BoardState *response) override;

    ::grpc::Status make_turn(::grpc::ServerContext *context,
                             const ::ttr::MakeTurnRequest *request,
                             ::ttr::MakeTurnResponse *response) override;
    BoardState local_get_board_state();

    MakeTurnResponse local_make_turn(const ttr::MakeTurnRequest *request);

private:
    TTRController *controller;
};

class LocalServer {
public:
    explicit LocalServer(TTRController *c);

    void runServer();

    void terminate();

private:
    std::unique_ptr<grpc::Server> server;
    TTRServer service;
    std::string server_address = "0.0.0.0:50051";
    grpc::ServerBuilder builder;
};

void RunServer(LocalServer *serv, bool needRun);
}  // namespace ttr
