//
// Created by megaserg01 on 25.02.2021.
//

#include "TTRController.h"
#include "Server/TTRServer.h"
void TTRController::start_game(int number_of_players,
                               int number_of_bots,
                               type_of_game type) {
    typeOfGame = type;
    if (typeOfGame != type_of_game::SINGLE_COMPUTER) {
        if (typeOfGame == type_of_game::LOCAL_SERVER) {
            game = new Game(number_of_players, number_of_bots);
        }
        std::cout<< "try to create client\n";
        client = new GameClient();
        my_id = client->get_id();
        std::cout<<"got id:"<<my_id<<'\n';
        if (my_id + 1 ==
            client->get_all_players().size()) {
            client->start_game();
            std::cout<<"game started, connected players: "<<my_id;
        }
    } else {
        game = new Game(number_of_players, number_of_bots);
        game->start_game();
    }
}

void TTRController::get_card_from_deck() {
    if (typeOfGame != type_of_game::LOCAL_CLIENT) {
        current_turn = new DrawCardFromDeck();
        Turn::increase_num();
        game->make_move(current_turn);
        if (Turn::num == 0)
            current_turn = nullptr;
    } else {
        // get response from server
    }
}

void TTRController::get_card_from_active(int num) {
    if (typeOfGame != type_of_game::LOCAL_CLIENT) {
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
    } else {
        // get response from server
    }
}

void TTRController::build_path_initialize(int id) {
    if (typeOfGame != type_of_game::LOCAL_CLIENT) {
        if (auto p = dynamic_cast<BuildStation *>(current_turn); p) {
            game->update_station_path(p->get_city(), id);
            current_turn = nullptr;
        }
        if (Turn::num == 0) {
            current_turn = new BuildPath(id);
        }
    } else {
        // get response from server
    }
}

void TTRController::get_routes() {
    if (typeOfGame != type_of_game::LOCAL_CLIENT) {
        current_turn = new TakeRoutes();
        game->make_move(current_turn);
        current_turn = nullptr;
    } else {
        // get response from server
    }
}

TTRController::~TTRController() {
    delete game;
    delete current_turn;
}

void TTRController::set_color_to_build_path(const WagonCard &w) {
    if (typeOfGame != type_of_game::LOCAL_CLIENT) {
        if (auto p = dynamic_cast<BuildPath *>(current_turn); p) {
            p->set_wagons(game->cards_with_suitable_color(
                w, game->players[game->active_player]));
            game->make_move(p);
            current_turn = nullptr;
        }
    } else {
        // get response from server
    }
}

std::vector<WagonCard> TTRController::get_current_player_cards() {
    if (typeOfGame == type_of_game::SINGLE_COMPUTER) {
        return game->players[game->active_player].wagon_cards;
    } else {
        if (typeOfGame == type_of_game::LOCAL_SERVER) {
            return game->players[my_id].wagon_cards;
        } else {
            return client->get_player_cards(my_id);
        }
    }
}

std::vector<Path> TTRController::get_paths() {
    if (typeOfGame != type_of_game::LOCAL_CLIENT) {
        return game->board.paths;
    } else {
        return client->get_paths();
    }
}

std::vector<WagonCard> TTRController::get_active_cards() {
    if (typeOfGame != type_of_game::LOCAL_CLIENT) {
        return game->deck.active_wagons;
    } else {
        return client->get_active_cards();
    }
}
std::map<std::string, int> TTRController::get_count_by_color() {
    if (typeOfGame != type_of_game::LOCAL_CLIENT) {
        return game->color_to_num();
    }
}
std::vector<Player> TTRController::get_players() {
    if (typeOfGame != type_of_game::LOCAL_CLIENT) {
        return game->players;
    } else {
        return client->get_all_players();
    }
}
int TTRController::is_game_end() {
    if (typeOfGame != type_of_game::LOCAL_CLIENT) {
        return game->check_end_game();
    } else {
        return 0;
    }
}
std::vector<int> TTRController::get_results() {
    if (typeOfGame != type_of_game::LOCAL_CLIENT) {
        game->count_players_points();
        std::vector<int> res;
        for (auto &i : game->players) {
            res.push_back(i.points);
        }
        return res;
    } else {
        // TODO also....
    }
}
std::vector<std::pair<std::string, Circle>> TTRController::get_stations() {
    if (typeOfGame != type_of_game::LOCAL_CLIENT) {
        std::vector<std::pair<std::string, Circle>> stations;
        for (const auto &i : game->cities) {
            stations.push_back({i.second, {i.first.x, i.first.y, 3}});
        }
        return stations;
    } else {
        // TODO
    }
}
void TTRController::build_station(const std::string &city) {
    if (typeOfGame != type_of_game::LOCAL_CLIENT) {
        if (!is_game_end()) {
            game->make_move(new BuildStation(city));
            current_turn = nullptr;
        } else {
            current_turn = new BuildStation(city);
        }
    } else {
        // TODO
    }
}
void TTRController::end_game() {
    if (typeOfGame != type_of_game::LOCAL_CLIENT) {
        if (typeOfGame == type_of_game::SINGLE_COMPUTER) {
            game->end_game();
        }
    }
}
int TTRController::get_current_player_id() {
    if (typeOfGame != type_of_game::LOCAL_CLIENT) {
        return game->active_player;
    } else {
        return client->get_id();
    }
}
std::vector<Path> TTRController::get_all_paths() {
    return get_paths();
}
bool TTRController::is_game_started() const {
    return started;
}
void TTRController::start_game_server() {
    assert(typeOfGame == type_of_game::LOCAL_SERVER);
    game->start_game();
}
