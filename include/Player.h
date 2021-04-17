//
// Created by timofey on 20.02.2021.
//

#ifndef PROJECT_PLAYER_H
#define PROJECT_PLAYER_H

#include <iostream>
#include <string>
#include <vector>
#include "Algo.h"
#include "Route.h"
#include "WagonCard.h"

struct Player {
private:
    static inline int id_ = 0;

public:
    static const int start_number_of_wagons = 45;
    static const int start_number_of_wagon_cards = 4;
    static const int start_number_of_short_routes = 3;
    static const int start_number_of_long_routes = 1;
    static const int start_number_of_stations = 3;
    int id;
    std::vector<WagonCard> wagon_cards;
    std::vector<Route> active_routes;
    std::vector<std::string> stations;
    std::set<int> station_paths;
    //(Max) print all integers after this comment(do not know about stations)
    int number_of_wagons_left;
    int points;
    int number_of_stations_left;
    int updated_stations = 0;
    Algo graph;
    Player();
    void count_points();
    void count_points_for_routes();
};

#endif  // PROJECT_PLAYER_H
