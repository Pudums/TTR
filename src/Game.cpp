//
// Created by timofey on 20.02.2021.
//

#include "../include/Game.h"
void Game::start_game() {
    for (auto& player : players) {
        player.wagon_cards = deck.get_start_wagon_cards();
        player.active_routes = deck.get_start_route_cards();
    }
    for (auto& player : players) {
        std::cout << player.id << '\n';
        for (const auto& elem : player.wagon_cards) {
            std::cout << elem.color << ' ';
        }
        std::cout << '\n';
        for (const auto& elem : player.active_routes) {
            std::cout << elem.city1 << ' ' << elem.city2 << ' ' << elem.points_for_passing << '\n';
        }
        std::cout << '\n';
    }
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

Game::Game(int number_of_players)
    : board(Board("paths.txt")),
      deck(Deck("wagons.txt", "short_routes.txt", "long_routes.txt")),
    discharge(Discharge()), players(std::vector<Player>(number_of_players)), active_player(0) {
}
