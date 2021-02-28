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
    game->make_move(t);
}

void TTRController::get_card_from_active(int num) {
    auto t = new DrawCardFromActive(num);
    game->make_move(t);
}

void TTRController::build_path_initialize(int id) {
    auto t = new BuildPath(id);
    game->make_move(t);
}

void TTRController::get_routes() {
    auto t = new DrawCardFromDeck();
    game->start_game();
}

