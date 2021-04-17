#include "protobuf/TTR.grpc.pb.h"
#include "protobuf/TTR.pb.h"
#include "TTRController.h"
#ifndef TTR_TTRSERVER_H
#define TTR_TTRSERVER_H
namespace ttr {
class TTRServer : virtual ttr::TTRService::Service{
public:
    TTRServer(TTRController *c);
    ::grpc::Status get_board_state(::grpc::ServerContext* context, const ::ttr::Nothing* request, ::ttr::BoardState* response) override;
    ::grpc::Status make_turn(::grpc::ServerContext* context, const ::ttr::MakeTurnRequest* request, ::ttr::MakeTurnResponse* response) override;
    BoardState local_get_board_state();
    MakeTurnResponse local_make_turn(const ttr::MakeTurnRequest* request);
private:
    std::unique_ptr<::ttr::TTRService::Stub> stub_;
    TTRController *controller;
};
}  // namespace ttr

#endif  // TTR_TTRSERVER_H
