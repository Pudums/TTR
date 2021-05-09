//
// Created by megaserg01 on 05.05.2021.
//

#ifndef TTR_CLIENT_H
#define TTR_CLIENT_H
#include <grpcpp/create_channel.h>
#include <exception>
#include <string>
#include <utility>
#include <vector>
#include "Path.h"
#include "TTR.grpc.pb.h"
class GameClient {
    std::unique_ptr<::ttr::TTRService::Stub> stub_;

public:
    explicit GameClient();
    ttr::BoardState *get_board_state();
    std::vector<Path> get_paths();
    int get_id();
    void start_game();
    std::vector<WagonCard> get_active_cards();
};
#endif  // TTR_CLIENT_H
