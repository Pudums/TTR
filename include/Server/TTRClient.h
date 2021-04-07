#include "../data/protobuf/TTR.grpc.pb.h"
#include "../data/protobuf/TTR.pb.h"

#ifndef TTR_TTRSERVER_H
#define TTR_TTRSERVER_H
namespace ttr {
class TTRServer {
public:
    explicit TTRServer();
    BoardState get_board_state();
    MakeTurnResponse make_turn_request(MakeTurnRequest);
private:
    std::unique_ptr<::ttr::TTRService::Stub> stub_;
};
}  // namespace ttr

#endif  // TTR_TTRSERVER_H
