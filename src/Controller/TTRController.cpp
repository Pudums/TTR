//
// Created by megaserg01 on 25.02.2021.
//

#include "TTRController.h"

void TTRController::start_game(int number_of_players) {
    game = new Game(number_of_players);
    game->start_game();
}

void TTRController::get_card_from_deck() {
    current_turn = new DrawCardFromDeck();
    Turn::increase_num();
    game->make_move(current_turn);
    if (Turn::num == 0)
        current_turn = nullptr;
}

void TTRController::get_card_from_active(int num) {
    if (game->deck.active_wagons[num].color == Multicolored and
        current_turn != nullptr) {
        return;
    } else if (game->deck.active_wagons[num].color == Multicolored) {
        Turn::num = 1;
    }
    current_turn = new DrawCardFromActive(num);
    Turn::increase_num();
    game->make_move(current_turn);

    if (Turn::num == 0)
        current_turn = nullptr;
}

void TTRController::build_path_initialize(int id) {
    if (Turn::num == 0) {
        current_turn = new BuildPath(id);
    }
}

void TTRController::get_routes() {
    current_turn = new TakeRoutes();
    game->make_move(current_turn);
    current_turn = nullptr;
}

TTRController::~TTRController() {
    delete game;
    delete current_turn;
}

void TTRController::set_color_to_build_path(const WagonCard &w) {
    if (auto p = dynamic_cast<BuildPath *>(current_turn); p) {
        p->set_wagons(game->cards_with_suitable_color(w));
        game->make_move(p);
        current_turn = nullptr;
    }
}

const std::vector<WagonCard> &TTRController::get_current_player_cards() {
    return game->players[game->active_player].wagon_cards;
}

std::vector<Path> TTRController::get_paths() {
    return game->board.paths;
}

std::vector<WagonCard> TTRController::get_active_cards() {
    return game->deck.active_wagons;
}
std::map<std::string, int> TTRController::get_count_by_color() {
    return game->color_to_num();
}
std::vector<int> TTRController::get_number_of_wagons() {
    std::vector<int> res;
    for (auto &player : game->players) {
        res.push_back(player.number_of_wagons_left);
    }
    return res;
}
