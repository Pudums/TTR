//
// Created by megaserg01 on 25.02.2021.
//

#include "TTRController.h"
#include "Server/TTRServer.h"
namespace {
int number_of_cards_with_fixed_color(const std::string &color,
                                     const std::vector<WagonCard> &cards) {
    int result = 0;
    for (const auto &wagon_card : cards) {
        if (wagon_card.color == color) {
            result++;
        }
    }
    return result;
}

}  // namespace
void TTRController::start_game(int number_of_players,
                               int number_of_bots,
                               type_of_game type) {
    typeOfGame = type;
    if (typeOfGame != type_of_game::SINGLE_COMPUTER) {
        if (typeOfGame == type_of_game::LOCAL_SERVER) {
            //server = new ttr::LocalServer(this);
            ttr::RunServer(this);
            game = new Game(number_of_players, number_of_bots);
            game->start_game();
        }
        std::cout << "try to create client\n";
        client = new GameClient();
        throw_exception_if_server_disconnected();
        my_id = client->get_id();

        std::cout << "got id:" << my_id << '\n';
        if (my_id + 1 == get_number_of_players()) {
            client->start_game();
            std::cout << "game started, connected players: " << my_id + 1;
        }
    } else {
        game = new Game(number_of_players, number_of_bots);
        game->start_game();
        started = true;
    }
}

void TTRController::get_card_from_deck(int id) {
    if(!is_game_started())return;
    if (typeOfGame != type_of_game::SINGLE_COMPUTER and
        (id == -1 and my_id != client->get_id() or
         id != -1 and id != client->get_id())) {
        std::cout << "it's not your turn: your id is" << my_id
                  << ", but now moves " << client->get_id();
        return;
    }
    current_turn = new DrawCardFromDeck();
    Turn::increase_num();
    if (typeOfGame != type_of_game::LOCAL_CLIENT) {
        game->make_move(current_turn);
    } else {
        throw_exception_if_server_disconnected();
        client->make_turn(current_turn, my_id);
    }
    if (Turn::num == 0)
        current_turn = nullptr;
}

void TTRController::get_card_from_active(int num, int id) {
    if(!is_game_started())return;
    if (typeOfGame != type_of_game::SINGLE_COMPUTER and
        (id == -1 and my_id != client->get_id() or
         id != -1 and id != client->get_id())) {
        std::cout << "it's not your turn: your id is" << my_id
                  << ", but now moves " << client->get_id();
        return;
    }
    if (typeOfGame != type_of_game::LOCAL_CLIENT) {
        if (game->deck.active_wagons[num].color == Multicolored and
            current_turn != nullptr) {
            return;
        } else if (game->deck.active_wagons[num].color == Multicolored) {
            Turn::num = 1;
        }
        current_turn = new DrawCardFromActive(num);
        Turn::increase_num();
        if (typeOfGame != type_of_game::LOCAL_CLIENT) {
            game->make_move(current_turn);
        } else {
            throw_exception_if_server_disconnected();
            client->make_turn(current_turn, my_id);
        }
        if (Turn::num == 0)
            current_turn = nullptr;
    } else {
        current_turn = new DrawCardFromActive(num);
        throw_exception_if_server_disconnected();
        client->make_turn(current_turn, my_id);
        current_turn = nullptr;
    }
}

void TTRController::build_path_initialize(int id, int player_id) {
    if(!is_game_started())return;
    if (typeOfGame != type_of_game::SINGLE_COMPUTER and
        (player_id == -1 and my_id != client->get_id() or
         player_id != -1 and player_id != client->get_id())) {
        std::cout << "it's not your turn: your id is" << my_id
                  << ", but now moves " << client->get_id();
        return;
    }
        if (auto p = dynamic_cast<BuildStation *>(current_turn); p) {
            p->set_path(id);
            std::cout<<"oh you want to build station!!!\n";
            if (typeOfGame != type_of_game::LOCAL_CLIENT) {
                    game->make_move(p);
            }else{
                throw_exception_if_server_disconnected();
                client->make_turn(p, my_id);
            }
            current_turn = nullptr;
        }

    if (Turn::num == 0) {
        current_turn = new BuildPath(id);
    }
}

void TTRController::get_routes(int id) {
    if (typeOfGame != type_of_game::SINGLE_COMPUTER and
        (id == -1 and my_id != client->get_id() or
                      id != -1 and id != client->get_id())) {
        std::cout << "it's not your turn: your id is" << my_id
                  << ", but now moves " << client->get_id();
        return;
    }
    current_turn = new TakeRoutes();
    if (typeOfGame != type_of_game::LOCAL_CLIENT) {
        game->make_move(current_turn);
        current_turn = nullptr;
    } else {
        throw_exception_if_server_disconnected();
        client->make_turn(current_turn, my_id);
    }
}

TTRController::~TTRController() {
    delete game;
    delete current_turn;
}

void TTRController::set_color_to_build_path(const WagonCard &w, int id) {
    if(!is_game_started())return;
    if(typeOfGame == type_of_game::LOCAL_CLIENT){
        throw_exception_if_server_disconnected();
    }
    if (typeOfGame != type_of_game::SINGLE_COMPUTER and
        (id == -1 and my_id != client->get_id() or
         id != -1 and id != client->get_id())) {
        std::cout << "it's not your turn: your id is" << my_id
                  << ", but now moves " << client->get_id();
        return;
    }
    if (typeOfGame != type_of_game::LOCAL_CLIENT) {
        if (auto p = dynamic_cast<BuildPath *>(current_turn); p) {
            p->set_wagons(game->cards_with_suitable_color(
                w, game->players[game->active_player]));
            game->make_move(p);
            current_turn = nullptr;
        }
    } else {
        if (auto p = dynamic_cast<BuildPath *>(current_turn); p) {
            p->set_wagons({w});
            throw_exception_if_server_disconnected();
            client->make_turn(p, my_id);
            current_turn = nullptr;
        } else {
            std::cout << "you should choose path first...\n";
        }
    }
}

std::vector<WagonCard> TTRController::get_current_player_cards() {
    if (typeOfGame == type_of_game::SINGLE_COMPUTER) {
        return game->players[game->active_player].wagon_cards;
    } else {
        throw_exception_if_server_disconnected();
        return client->get_player_cards(my_id);
    }
}

std::vector<Path> TTRController::get_paths() {
    if (typeOfGame != type_of_game::LOCAL_CLIENT) {
        return game->board.paths;
    } else {
        throw_exception_if_server_disconnected();
        return client->get_paths();
    }
}

std::vector<WagonCard> TTRController::get_active_cards() {
    if (typeOfGame != type_of_game::LOCAL_CLIENT) {
        return game->deck.active_wagons;
    } else {
        throw_exception_if_server_disconnected();
        return client->get_active_cards();
    }
}

std::map<std::string, int> TTRController::get_count_by_color() {
    if (typeOfGame == type_of_game::SINGLE_COMPUTER) {
        return game->color_to_num();
    } else {
        std::map<std::string, int> result;
        std::vector<std::string> colors{White,  Orange, Green,
                                        Red,    Black,  Blue,
                                        Yellow, Purple, Multicolored};
        for (const auto &color : colors) {
            result[color] = number_of_cards_with_fixed_color(
                color, get_current_player_cards());
        }
        return result;
    }
}

std::vector<Player> TTRController::get_players() {
    if (typeOfGame != type_of_game::LOCAL_CLIENT) {
        return game->players;
    } else {
        throw_exception_if_server_disconnected();
        return client->get_all_players();
    }
}

int TTRController::is_game_end() {
    if (typeOfGame != type_of_game::LOCAL_CLIENT) {
        return game->check_end_game();
    } else {
        throw_exception_if_server_disconnected();
        return client->is_game_end();
    }
}
//🥰🥰🥰🥰🥰🥰🥰🥰


std::vector<int> TTRController::get_results() {
    if (typeOfGame != type_of_game::LOCAL_CLIENT) {
        game->count_players_points();
        std::vector<int> res;
        for (auto &i : game->players) {
            res.push_back(i.points);
        }
        return res;
    } else {
        throw_exception_if_server_disconnected();
        return client->get_score();
    }
}

std::vector<std::pair<std::string, Circle>> TTRController::get_stations() {
    if (typeOfGame != type_of_game::LOCAL_CLIENT) {
        std::vector<std::pair<std::string, Circle>> stations;
        for (const auto &i : game->cities) {
            stations.push_back({i.second, {i.first.p.x, i.first.p.y, i.first.r}});
        }
        return stations;
    } else {
        throw_exception_if_server_disconnected();
        return client->get_stations();
    }
}

void TTRController::build_station(const std::string &city, int id) {
    if(typeOfGame == type_of_game::LOCAL_CLIENT){
        throw_exception_if_server_disconnected();
    }
    if (typeOfGame != type_of_game::SINGLE_COMPUTER and
        (id == -1 and my_id != client->get_id() or
                      id != -1 and id != client->get_id())) {
        std::cout << "it's not your turn: your id is" << my_id
                  << ", but now moves " << client->get_id();
        return;
    }
    current_turn = new BuildStation(city);
}

void TTRController::end_game() {
    if (typeOfGame != type_of_game::LOCAL_CLIENT) {
            game->end_game();
    }
}

int TTRController::get_current_player_id() {
    if (typeOfGame != type_of_game::LOCAL_CLIENT) {
        return game->active_player;
    } else {
        throw_exception_if_server_disconnected();
        return client->get_id();
    }
}


std::vector<Path> TTRController::get_all_paths() {
    return get_paths();
}

bool TTRController::is_game_started() const {
    if(typeOfGame != type_of_game::LOCAL_CLIENT){
        return started;
    }else{
        return true;
    }
}

void TTRController::start_game_server() {
    started = true;
}


int TTRController::get_number_of_players() {
    if (typeOfGame != type_of_game::LOCAL_CLIENT) {
        return game->number_of_players;
    } else {
        throw_exception_if_server_disconnected();
        return client->get_number_of_players();
    }
}

int TTRController::get_my_id(){
    if(typeOfGame == type_of_game::SINGLE_COMPUTER){
        return get_current_player_id();
    }
    return my_id;
}
void TTRController::throw_exception_if_server_disconnected() {
    if(!*client){
        throw std::logic_error("cannot connect to server");
    }
}
bool TTRController::is_my_turn() {
    if(typeOfGame == type_of_game::SINGLE_COMPUTER){
        return true;
    }
    return my_id == get_current_player_id();
}
