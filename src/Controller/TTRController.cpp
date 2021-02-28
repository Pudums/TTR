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
