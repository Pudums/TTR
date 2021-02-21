//
// Created by timofey on 20.02.2021.
//

#include "Game.h"
void Game::start() const {
}

bool Game::check_end_game() const {
    for (const auto &player : players) {
        if (player.number_of_wagons_left <= Game::number_of_wagons_for_finish) {
            return true;
        }
    }
    return false;
}

void Game::end_game() const {
}
