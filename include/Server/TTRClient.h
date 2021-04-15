#include "protobuf/TTR.grpc.pb.h"
#include "protobuf/TTR.pb.h"
#include "TTRController.h"
#ifndef TTR_TTRSERVER_H
#define TTR_TTRSERVER_H
namespace ttr {
class TTRServer{
public:
    TTRServer(TTRController *c);
    BoardState get_board_state();
    MakeTurnResponse make_turn_request(const MakeTurnRequest &request);

private:
    std::unique_ptr<::ttr::TTRService::Stub> stub_;
    TTRController *controller;
};
}  // namespace ttr

#endif  // TTR_TTRSERVER_H
