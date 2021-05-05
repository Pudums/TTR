#include "Server/Client.h"
GameClient::GameClient() {
    stub_ = ::ttr::TTRService::NewStub(grpc::CreateChannel("localhost:50051",
                                                           grpc::InsecureChannelCredentials()));

}
