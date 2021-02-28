//
// Created by megaserg01 on 25.02.2021.
//

#include "Controller/TTRController.h"

void TTRController::set_data() {
    data = "hello";
    get_data_to_game();
}
void TTRController::get_data_to_game() {
    game->make_move(data);
}
void TTRController::start_game(int number_of_players) {
    new(game) Game(number_of_players);
    game->start_game();
}
