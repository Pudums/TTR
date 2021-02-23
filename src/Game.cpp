//
// Created by timofey on 20.02.2021.
//

#include "../include/Game.h"
namespace {
int number_of_locomotives(const std::vector<WagonCard> &list_of_wagons) {
    int counter = 0;
    for (const auto &elem : list_of_wagons) {
        if (elem.color == "Multicolored") {
            counter++;
        }
    }
    return counter;
}

bool one_type_wagons(const std::vector<WagonCard> &list_of_wagons) {
    std::set<std::string> color_types;
    for (const auto &elem : list_of_wagons) {
        color_types.insert(elem.color);
    }
    if (color_types.size() > 2 ||
        (color_types.size() == 2 &&
         number_of_locomotives(list_of_wagons) == 0)) {
        return false;
    }
    return true;
}

bool is_building_path_correct(const Path &path,
                              const std::vector<WagonCard> &list_of_wagons) {
    if (path.length != list_of_wagons.size() ||
        path.number_of_locomotives > number_of_locomotives(list_of_wagons) ||
        !one_type_wagons(list_of_wagons)) {
        return false;
    }
    int number_of_wagons_same_type = 0;
    for (const auto &elem : list_of_wagons) {
        if (elem.color == path.color) {
            number_of_wagons_same_type++;
        }
    }
    if (path.color == "Uncolored" ||
        number_of_wagons_same_type + number_of_locomotives(list_of_wagons) ==
            path.length) {
        return true;
    }
    return false;
}
}  // namespace

void Game::start_game() {
    for (auto &player : players) {
        player.wagon_cards = deck.get_start_wagon_cards();
        player.active_routes = deck.get_start_route_cards();
    }
    for (auto &player : players) {
        std::cout << player.id << '\n';
        for (const auto &elem : player.wagon_cards) {
            std::cout << elem.color << ' ';
        }
        std::cout << '\n';
        for (const auto &elem : player.active_routes) {
            std::cout << elem.city1 << ' ' << elem.city2 << ' '
                      << elem.points_for_passing << '\n';
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
      discharge(Discharge()),
      players(std::vector<Player>(number_of_players)),
      active_player(1) {
}
