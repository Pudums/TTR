//
// Created by megaserg01 on 25.02.2021.
//

#include "Controller/TTRController.h"


void TTRController::start_game(int number_of_players) {
    new(game) Game(number_of_players);
    game->start_game();
}

void TTRController::get_card_from_deck() {
    auto t = new DrawCardFromDeck();
}
void TTRController::get_card_from_active(int num) {
    auto t = new DrawCardFromActive(num);
}
void TTRController::build_path_initialize(int id) {
    auto t = new BuildPath(id);
}
void TTRController::get_routes() {
    auto t = new DrawCardFromDeck();
}
