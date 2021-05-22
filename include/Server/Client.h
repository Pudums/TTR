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
#include "Player.h"
#include "TTR.grpc.pb.h"
class GameClient {
    std::unique_ptr<::ttr::TTRService::Stub> stub_;
    ttr::PlayerState get_state(int id);
    ttr::BoardState *get_board_state();

public:
    explicit GameClient();
    std::vector<Path> get_paths();
    int get_id();
    void start_game();
    int get_number_of_players();
    std::vector<std::pair<std::string, Circle>> get_stations();
    std::vector<WagonCard> get_active_cards();
    std::vector<WagonCard> get_player_cards(int id);
    std::vector<Player> get_all_players();
    std::vector<int> get_score();
    void make_turn(Turn *turn, int id);
    int is_game_end();
};
#endif  // TTR_CLIENT_H
