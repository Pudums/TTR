//
// Created by timofey on 20.02.2021.
//

#include "../include/Game.h"
namespace {
struct ListOfCardsForPath {
    std::string color = None;
    int number_of_colored_wagons = 0;
    int number_of_locomotives = 0;
};

ListOfCardsForPath comfortable_format(
    const std::vector<WagonCard> &list_of_wagon_cards) {
    ListOfCardsForPath result;
    for (const auto &wagon_card : list_of_wagon_cards) {
        if (result.color == None && wagon_card.color != Multicolored) {
            result.color = wagon_card.color;
        } else if (wagon_card.color != Multicolored) {
            result.number_of_colored_wagons++;
        } else {
            result.number_of_locomotives++;
        }
    }
    return result;
}

void create_graphs_for_players(std::vector<Player> &players,
                               const std::vector<Path> &paths) {
    for (auto &player : players) {
        player.graph = Algo(paths, player.id, player.station_paths);
    }
}

std::vector<std::pair<Point, std::string>> read_cities(
    const std::string &filename) {
    std::vector<std::pair<Point, std::string>> cities;
    std::ifstream is(filename);
    for (int i = 0; i < 47; i++) {
        std::string name;
        int x, y;
        is >> name >> x >> y;
        cities.push_back({{x, y}, name});
    }
    return cities;
}

}  // namespace

void Game::start_game() {
    for (auto &player : players) {
        player.wagon_cards = deck.get_start_wagon_cards();
        player.active_routes = deck.get_start_route_cards();
    }
    if (players[active_player].is_bot) {
        make_move(nullptr);
    }
}

int Game::check_end_game() const {
    if (number_updated_players == players.size()) {
        return 2;
    }
    for (const auto &player : players) {
        if (player.number_of_wagons_left <= Game::number_of_wagons_for_finish) {
            return 1;
        }
    }
    return 0;
}

Game::Game(int number_of_players, int number_of_bots)
    : board(Board("data/paths.txt", "data/wagon_blocks.txt")),
      discharge(Discharge()),
      deck(Deck("data/wagons.txt",
                "data/short_routes.txt",
                "data/long_routes.txt",
                discharge)),
      active_player(0),
      number_of_players(number_of_players),
      cities(read_cities("data/cities.txt")) {
    std::cout << "players " << number_of_players - number_of_bots << " bots "
              << number_of_bots << std::endl;
    for (int i = 0; i < number_of_players - number_of_bots; i++) {
        players.emplace_back(false);
    }
    for (int i = 0; i < number_of_bots; i++) {
        players.emplace_back(true);
    }
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

bool Game::move_build_station(const std::string &city) {
    if (players[active_player].number_of_stations_left == 0 ||
        occupied_stations.find(city) != occupied_stations.end()) {
        return false;
    }
    players[active_player].number_of_stations_left--;
    players[active_player].stations.push_back(city);
    occupied_stations[city] = active_player;
    return true;
}

bool Game::check_if_enough_cards_for_building_path(
    const Path &path,
    const std::vector<WagonCard> &list_of_cards) const {
    for (const auto &elem : list_of_cards) {
        if (elem.color != Multicolored && path.color != Uncolored &&
            path.color != elem.color) {
            return false;
        }
    }
    if (path.length > list_of_cards.size() ||
        path.number_of_locomotives >
            number_of_cards_with_fixed_color(Multicolored)) {
        return false;
    }
    return true;
}

void Game::add_extra_tunnel_cards(Path &path) {
    std::vector<WagonCard> extra_tunnel_cards = deck.get_cards_for_tunnel();
    for (const auto &extra_card : extra_tunnel_cards) {
        if (extra_card.color == Multicolored ||
            extra_card.color == path.color) {
            path.number_of_colored_wagons++;
        }
    }
}

bool Game::move_build_path(int position,
                           const std::vector<WagonCard> &list_of_wagon_cards) {
    Path path = board.paths[position];
    if (path.is_tunnel) {
        add_extra_tunnel_cards(path);
    }
    if (check_if_enough_cards_for_building_path(board.paths[position],
                                                list_of_wagon_cards)) {
        update_state_after_path_building(board.paths[position],
                                         list_of_wagon_cards);
        return true;
    }
    return false;
}

std::vector<WagonCard> Game::cards_with_suitable_color(
    const WagonCard &wagon_card, const Player& player) const {
    std::vector<WagonCard> result;
    for (const auto &elem : player.wagon_cards) {
        if (elem.color == wagon_card.color || elem.color == Multicolored) {
            result.push_back(elem);
        }
    }
    return result;
}

void Game::update_state_after_path_building(
    Path &path,
    const std::vector<WagonCard> &list_of_wagon_cards) {
    path.owner = active_player;
    players[active_player].points += Path::points_for_path(path.length);
    players[active_player].number_of_wagons_left -= path.length;
    ListOfCardsForPath result = comfortable_format(list_of_wagon_cards);
    int remove_colored_cards_left = path.number_of_colored_wagons;
    int remove_locomotives_left = path.number_of_locomotives;
    int current_pos = 0;
    while (current_pos < players[active_player].wagon_cards.size()) {
        if (remove_colored_cards_left > 0 &&
            players[active_player].wagon_cards[current_pos].color ==
                result.color) {
            discharge.deck.push_back(
                players[active_player].wagon_cards[current_pos]);
            players[active_player].wagon_cards.erase(
                players[active_player].wagon_cards.begin() + current_pos);
            remove_colored_cards_left--;
        } else {
            current_pos++;
        }
    }
    current_pos = 0;
    while (current_pos < players[active_player].wagon_cards.size()) {
        if (remove_colored_cards_left > 0 &&
            players[active_player].wagon_cards[current_pos].color ==
                Multicolored) {
            discharge.deck.push_back(
                players[active_player].wagon_cards[current_pos]);
            players[active_player].wagon_cards.erase(
                players[active_player].wagon_cards.begin() + current_pos);
            remove_colored_cards_left--;
        } else if (remove_locomotives_left > 0 &&
                   players[active_player].wagon_cards[current_pos].color ==
                       Multicolored) {
            discharge.deck.push_back(
                players[active_player].wagon_cards[current_pos]);
            players[active_player].wagon_cards.erase(
                players[active_player].wagon_cards.begin() + current_pos);
            remove_locomotives_left--;
        } else {
            current_pos++;
        }
    }
}

void Game::make_move(Turn *t) {
    bool flag = true;
    if (auto *p = dynamic_cast<DrawCardFromDeck *>(t); p) {
        get_wagon_card_from_deck();
    }  // OK
    if (auto *p = dynamic_cast<DrawCardFromActive *>(t); p) {
        get_wagon_card_from_active_cards(p->number);
    }  // OK
    if (auto *p = dynamic_cast<TakeRoutes *>(t); p) {
        move_get_new_roots();
    }  // OK
    if (auto *p = dynamic_cast<BuildStation *>(t); p) {
        if (!move_build_station(p->get_city())) {
            flag = false;
        }
    }
    if (auto *p = dynamic_cast<BuildPath *>(t); p) {
        if (!move_build_path(p->get_pos(), p->getWagons())) {
            flag = false;
        }
    }  // OK
    if (Turn::num == 0 && flag) {
        active_player = (active_player + 1) % number_of_players;
    }
    while (players[active_player].is_bot) {
        std::cout << board.paths[1].color << ' ' << board.paths[1].length
                  << std::endl;
        for (int i = 0; i < players[active_player].wagon_cards.size(); i++) {
            std::cout << players[active_player].wagon_cards[i].color << ' ';
        }
        std::cout << std::endl;
        std::set<std::string> player_cities = players_cities();
        // int path_pos =
        // Algo::find_best_way(players[active_player].active_routes[0].city2,
        // player_cities, board.paths);
        std::vector<WagonCard> needed_cards = cards_with_suitable_color(WagonCard(board.paths[1].color), players[active_player]);
        if (check_if_enough_cards_for_building_path(board.paths[1],
                                                    needed_cards)) {
            bool f = move_build_path(
                1, needed_cards);
        } else {
            get_wagon_card_from_deck();
            get_wagon_card_from_deck();
        }
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
}

int Game::number_of_cards_with_fixed_color(const std::string &color) const {
    int result = 0;
    for (const auto &wagon_card : players[active_player].wagon_cards) {
        if (wagon_card.color == color) {
            result++;
        }
    }
    return result;
}

std::map<std::string, int> Game::color_to_num() const {
    std::map<std::string, int> result;
    std::vector<std::string> colors{White, Orange, Green,  Red,         Black,
                                    Blue,  Yellow, Purple, Multicolored};
    for (const auto &color : colors) {
        result[color] = number_of_cards_with_fixed_color(color);
    }
    return result;
}

void Game::update_station_path(const std::string &station_city, int path_pos) {
    Path path = board.paths[path_pos];
    if (path.start == station_city || path.finish == station_city) {
        players[active_player].station_paths.insert(path_pos);
        players[active_player].updated_stations++;
    }
    while (players[active_player].updated_stations ==
           Player::start_number_of_stations -
               players[active_player].number_of_stations_left) {
        active_player = (active_player + 1) % number_of_players;
        number_updated_players++;
        if (number_of_players == players.size()) {
            break;
        }
    }
}

std::set<std::string> Game::players_cities() {
    std::set<std::string> visited_cities;
    for (const auto &elem : board.paths) {
        if (elem.owner == active_player) {
            visited_cities.insert(elem.start);
            visited_cities.insert(elem.finish);
        }
    }
    return visited_cities;
}
