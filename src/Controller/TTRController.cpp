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
    game->make_move(current_turn);
}

void TTRController::get_card_from_active(int num) {
    current_turn = new DrawCardFromActive(num);
    game->make_move(current_turn);
}

void TTRController::build_path_initialize(int id) {
    current_turn = new BuildPath(id);
}

void TTRController::get_routes() {
    current_turn = new DrawCardFromDeck();
    game->make_move(current_turn);
}

TTRController::~TTRController() {
    delete game;
    delete current_turn;
}

void TTRController::set_color_to_build_path(const WagonCard &w) {
    if (auto p = dynamic_cast<BuildPath *>(current_turn); p) {
        p->set_wagons(game->cards_with_suitable_color(w));
        game->make_move(p);
    }
}

const std::vector<WagonCard> &TTRController::get_current_player_cards() {
    return game->players[game->active_player].wagon_cards;
}

std::vector<Path> TTRController::get_paths() {
    return game->board.paths;
}
