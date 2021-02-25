#include "TTRClient.h"
#include <grpc/grpc.h>
#include <grpcpp/channel.h>
#include <grpcpp/client_context.h>
#include <grpcpp/create_channel.h>
#include <grpcpp/security/credentials.h>
//#include "Controller.h" - need to implement
using grpc::Channel;
using grpc::ClientContext;
using grpc::ClientReader;
using grpc::ClientReaderWriter;
using grpc::ClientWriter;
using grpc::Status;
namespace ttr {

TTRServer::TTRServer() {
    stub_ = TTRService::NewStub(grpc::CreateChannel(
        "localhost:50051", grpc::InsecureChannelCredentials()));
}
}  // namespace ttr