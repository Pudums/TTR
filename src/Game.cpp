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

std::vector<std::pair<Circle, std::string>> read_cities(
    const std::string &filename) {
    std::vector<std::pair<Circle, std::string>> cities;
    std::ifstream is(filename);
    for (int i = 0; i < 47; i++) {
        std::string name;
        int x, y;
        is >> name >> x >> y;
        cities.push_back({{{x, y}, -1}, name});
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
            return 2;
            // return 1
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

bool Game::get_wagon_card_from_deck() {
    if (deck.wagons_deck.empty()) {
        return false;
    }
    players[active_player].wagon_cards.push_back(deck.draw_card_from_deck());
    return true;
}

bool Game::get_wagon_card_from_active_cards(int position) {
    if (deck.active_wagons[position].color.empty()) {
        return false;
    }
    players[active_player].wagon_cards.push_back(
        deck.draw_card_from_active_cards(position));
    return true;
}

bool Game::move_build_station(const std::string &city, int path_id) {
    if (players[active_player].number_of_stations_left == 0 ||
        occupied_stations.find(city) != occupied_stations.end() ||
        board.paths[path_id].owner == -1) {
        return false;
    }
    players[active_player].number_of_stations_left--;
    players[active_player].station_paths.insert(path_id);
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
    if (path.owner != -1) {
        return false;
    }
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
    const WagonCard &wagon_card,
    const Player &player) {
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
    std::cout << "players move" << std::endl;
    bool flag = true;
    if (auto *p = dynamic_cast<DrawCardFromDeck *>(t); p) {
        std::cout << "draw card from deck" << std::endl;
        if (!get_wagon_card_from_deck()) {
            flag = false;
        }
    }  // OK
    if (auto *p = dynamic_cast<DrawCardFromActive *>(t); p) {
        std::cout << "draw card from active" << std::endl;
        if (!get_wagon_card_from_active_cards(p->number)) {
            flag = false;
            std::cout << "no active cards" << std::endl;
        }
    }  // OK
    if (auto *p = dynamic_cast<TakeRoutes *>(t); p) {
        std::cout << "draw routes" << std::endl;
        move_get_new_roots();
    }  // OK
    if (auto *p = dynamic_cast<BuildStation *>(t); p) {
        std::cout << "build station" << std::endl;
        if (!move_build_station(p->get_city(), p->get_path_pos())) {
            flag = false;
        }
    }
    if (auto *p = dynamic_cast<BuildPath *>(t); p) {
        std::cout << "build path" << std::endl;
        if (!move_build_path(p->get_pos(), p->getWagons())) {
            flag = false;
        }
    }  // OK
    if (check_end_game()) {
        std::cout << "end game" << std::endl;
        end_game();
    }
    if (Turn::num == 0 && flag) {
        active_player = (active_player + 1) % number_of_players;
    }
    while (players[active_player].is_bot) {
        std::cout << "bot moves: " << active_player << std::endl;
        int route_pos = 0;
        std::set<std::string> player_cities = players_cities();
        bool moved = false;
        while (player_cities.find(
                   players[active_player].active_routes[route_pos].city2) !=
               player_cities.end()) {
            route_pos++;
            if (route_pos == player_cities.size()) {
                std::cout << "take routes" << std::endl;
                move_get_new_roots();
                moved = true;
                break;
            }
        }
        if (!moved) {
            std::cout << "finding best way   ";
            player_cities.insert(
                players[active_player].active_routes[route_pos].city1);
            int path_pos = Algo::find_best_way(
                players[active_player].active_routes[route_pos].city2,
                player_cities, board.paths);
            std::cout << path_pos << std::endl;
            if (path_pos > 1000) {
                std::cout
                    << players[active_player].active_routes[route_pos].city2
                    << std::endl;
                for (auto elem : player_cities) {
                    std::cout << elem << ' ';
                }
                std::cout << std::endl;
            }
            std::cout << board.paths[path_pos].color << std::endl;
            std::vector<WagonCard> needed_cards = cards_with_suitable_color(
                WagonCard(board.paths[path_pos].color), players[active_player]);
            if (check_if_enough_cards_for_building_path(board.paths[path_pos],
                                                        needed_cards) &&
                board.paths[path_pos].owner == -1) {
                std::cout << "build path" << std::endl;
                bool f = move_build_path(path_pos, needed_cards);
            } else {
                std::cout << "draw cards" << std::endl;
                get_wagon_card_from_deck();
                get_wagon_card_from_deck();
            }
        }
        active_player = (active_player + 1) % number_of_players;
        std::cout << "turn is finished" << std::endl;
    }
}

void Game::count_players_points() {
    for (auto &player : players) {
        player.count_points();
        std::cout << player.id << std::endl;
    }
}

void Game::end_game() {
    std::cout << "finishing game" << std::endl;
    create_graphs_for_players(players, board.paths);
    std::cout << "counting points" << std::endl;
    count_players_points();
    std::cout << "finished" << std::endl;
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
