//
// Created by timofey on 20.02.2021.
//
//
#include "../include/Player.h"
Player::Player()
    : id(id_++), number_of_wagons_left(Player::start_number_of_wagons), points(0) {
}
void Player::count_points_for_routes() {
    for (const auto& route : active_routes) {
        if (graph.is_route_exists(route.city1, route.city2)) {
            points += route.points_for_passing;
        }
    }
}

void Player::count_points() {
    count_points_for_routes();
}
