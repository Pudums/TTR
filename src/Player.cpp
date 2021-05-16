//
// Created by timofey on 20.02.2021.
//
//
#include "../include/Player.h"
Player::Player(bool is_bot)
    : id(id_++),
      number_of_wagons_left(Player::start_number_of_wagons),
      points(0),
      number_of_stations_left(start_number_of_stations),
      is_bot(is_bot) {
}

void Player::count_points_for_routes() {
    for (const auto &route : active_routes) {
        bool f;
        std::cout << "start counting" << std::endl;
        f = graph.is_route_exists(route.city1, route.city2);
        if (f) {
            std::cout << "route points " << route.points_for_passing
                      << std::endl;
            points += route.points_for_passing;
        } else {
            std::cout << "route is unfinished" << std::endl;
        }
    }
}

void Player::count_points() {
    count_points_for_routes();
}
