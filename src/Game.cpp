//
// Created by timofey on 20.02.2021.
//

#include "../include/Game.h"
namespace {
void create_graphs_for_players(std::vector<Player> &players,
                               const std::vector<Path> &paths) {
    for (auto &player : players) {
        player.graph = Algo(paths, player.id);
    }
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
    // make_move();
}

bool Game::check_end_game() const {
    for (const auto &player : players) {
        if (player.number_of_wagons_left <= Game::number_of_wagons_for_finish) {
            return true;
        }
    }
    return false;
}

Game::Game(int number_of_players)
    : board(Board("data/paths.txt")),
      discharge(Discharge()),
      deck(Deck("data/wagons.txt",
                "data/short_routes.txt",
                "data/long_routes.txt",
                discharge)),
      players(std::vector<Player>(number_of_players)),
      active_player(0),
      number_of_players(number_of_players) {
}

void Game::move_get_new_roots() {
    std::vector<Route> new_routes = deck.get_new_routes();
    players[active_player].active_routes.insert(
        players[active_player].active_routes.end(), new_routes.begin(),
        new_routes.end());
}

void Game::get_wagon_card_from_deck() {
    players[active_player].wagon_cards.push_back(deck.draw_card_from_deck());
}

void Game::get_wagon_card_from_active_cards(int position) {
    players[active_player].wagon_cards.push_back(
        deck.draw_card_from_active_cards(position));
}

void Game::move_build_path(int position,
                           const std::vector<WagonCard> &list_of_wagon_cards) {
    // TODO
}

std::vector<WagonCard> Game::cards_with_suitable_color(
    const WagonCard &wagon_card) const {
    std::vector<WagonCard> result;
    for (const auto& elem : players[active_player].wagon_cards) {
        if (elem.color == wagon_card.color || elem.color == "Multicolored") {
            result.push_back(elem);
        }
    }
    return result;
}

void Game::update_state_after_path_building(Path &path, Player &player) {
    path.owner = active_player;
    player.points += Path::points_for_path(path.length);
}

void Game::make_move(Turn *t) {
    if (auto *p = dynamic_cast<DrawCardFromDeck *>(t)) {
        get_wagon_card_from_deck();
    }
    if (auto *p = dynamic_cast<DrawCardFromActive *>(t)) {
        get_wagon_card_from_active_cards(p->number);
    }
    if (auto p = dynamic_cast<TakeRoutes *>(t)) {
        move_get_new_roots();
    }
    if (auto *p = dynamic_cast<BuildPath *>(t)) {
        move_build_path(p->get_pos(), p->getWagons());
    }
    if (Turn::num == 0) {
        active_player = (active_player + 1) % number_of_players;
    }
}

void Game::count_players_points() {
    for (auto &player : players) {
        player.count_points();
    }
}

void Game::end_game() {
    create_graphs_for_players(players, board.paths);
    count_players_points();
    // TODO звершение
}
