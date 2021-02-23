//
// Created by timofey on 20.02.2021.
//

#ifndef PROJECT_GAME_H
#define PROJECT_GAME_H

#include <iostream>
#include <vector>
#include "Board.h"
#include "Deck.h"
#include "Discharge.h"
#include "Player.h"

struct Game {
private:
    static const int number_of_wagons_for_finish = 2;

public:
    Board board;
    Deck deck;
    Discharge discharge;
    std::vector<Player> players;
    int active_player;

    Game(int number_of_players);

    void start_game();
    [[nodiscard]] bool check_end_game() const;
    void end_game() const;
};

#endif  // PROJECT_GAME_H
