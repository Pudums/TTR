//
// Created by timofey on 20.02.2021.
//

#ifndef PROJECT_GAME_H
#define PROJECT_GAME_H

#include <cassert>
#include <iostream>
#include <set>
#include <string>
#include <vector>
#include "Board.h"
#include "Deck.h"
#include "Discharge.h"
#include "Player.h"
#include "Algo.h"

struct Game {
private:
    static const int number_of_wagons_for_finish = 2;
    static const int number_of_getting_wagons = 2;

public:
    Board board;
    Discharge discharge;
    Deck deck;
    std::vector<Player> players;
    int active_player;
    int number_of_players;

    explicit Game(int number_of_players);

    void start_game();
    void make_move();
    void move_get_new_roots();
    void move_get_new_wagon_cards();
    void move_build_path(int position);
    void get_wagon_card_from_deck();
    void get_wagon_card_from_active_cards(int position);
    void update_state_after_path_building(Path& path, Player &player);
    [[nodiscard]] bool check_end_game() const;
    void end_game();
    void count_players_points();
};

#endif  // PROJECT_GAME_H
