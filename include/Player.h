//
// Created by timofey on 20.02.2021.
//

#ifndef PROJECT_PLAYER_H
#define PROJECT_PLAYER_H

#include <iostream>
#include <string>
#include <vector>
#include "Route.h"
#include "WagonCard.h"

struct Player {
private:
    static const int start_number_of_wagons = 45;
    static const int start_number_of_wagon_cards = 4;
    static const int start_number_of_short_routes = 3;
    static const int start_number_of_long_routes = 1;

public:
    std::string color;
    std::vector<WagonCard> wagon_cards;
    std::vector<Route> active_routes;
    int number_of_wagons_left;

};

#endif  // PROJECT_PLAYER_H
