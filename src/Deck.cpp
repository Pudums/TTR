//
// Created by timofey on 20.02.2021.
//

#include "../include/Deck.h"
#include <random>

namespace {
std::vector<WagonCard> parse_wagons_file(std::ifstream &list_of_wagons) {
    std::vector<WagonCard> wagons_list;
    std::string color;
    int number_of_wagons;
    while (list_of_wagons >> color) {
        list_of_wagons >> number_of_wagons;
        for (int i = 0; i < number_of_wagons; i++) {
            wagons_list.push_back({color});
        }
    }
    std::shuffle(wagons_list.begin(), wagons_list.end(),
                 std::mt19937(std::random_device()()));
    return wagons_list;
}

std::vector<Route> parse_routes_file(std::ifstream &list_of_routes) {
    std::vector<Route> routes_list;
    std::string city1;
    std::string city2;
    int points_for_passing;
    while (list_of_routes >> city1) {
        list_of_routes >> city2;
        list_of_routes >> points_for_passing;
        routes_list.push_back({city1, city2, points_for_passing});
    }
    std::shuffle(routes_list.begin(), routes_list.end(),
                 std::mt19937(std::random_device()()));
    return routes_list;
}
}  // namespace

Deck::Deck(const std::string &wagons_file_name,
           const std::string &short_routes_file_name,
           const std::string &long_routes_file_name) {
    std::ifstream list_of_wagons(wagons_file_name);
    std::ifstream list_of_short_routes(short_routes_file_name);
    std::ifstream list_of_long_routes(long_routes_file_name);
    wagons_deck = parse_wagons_file(list_of_wagons);
    set_start_active_wagons();
    short_routes = parse_routes_file(list_of_short_routes);
    long_routes = parse_routes_file(list_of_long_routes);
    for (const auto &elem : wagons_deck) {
        std::cout << elem.color << '\n';
    }
    std::cout << '\n';
    for (const auto &elem : active_wagons) {
        std::cout << elem.color << '\n';
    }
    std::cout << '\n';
    for (const auto &elem : short_routes) {
        std::cout << elem.city1 << ' ' << elem.city2 << ' '
                  << elem.points_for_passing << '\n';
    }
    std::cout << '\n';
    for (const auto &elem : long_routes) {
        std::cout << elem.city1 << ' ' << elem.city2 << ' '
                  << elem.points_for_passing << '\n';
    }
    std::cout << '\n';
}

int Deck::wagons_deck_size() const {
    return wagons_deck.size();
}

bool Deck::is_wagons_deck_empty() const {
    return wagons_deck.empty();
}

int Deck::routes_deck_size() const {
    return short_routes.size();
}

bool Deck::is_routes_deck_empty() const {
    return short_routes.empty();
}

void Deck::set_start_active_wagons() {
    active_wagons = std::vector<WagonCard>(Deck::number_of_active_cards);
    for (int i = 0; i < Deck::number_of_active_cards; i++) {
        active_wagons[i] = wagons_deck.back();
        wagons_deck.pop_back();
    }
}
std::vector<WagonCard> Deck::get_start_wagon_cards() {
    std::vector<WagonCard> result(Player::start_number_of_wagon_cards);
    for (int i = 0; i < Player::start_number_of_wagon_cards; i++) {
        result[i] = wagons_deck.back();
        wagons_deck.pop_back();
    }
    return result;
}
std::vector<Route> Deck::get_start_route_cards() {
    std::vector<Route> result(Player::start_number_of_short_routes +
                              Player::start_number_of_long_routes);
    for (int i = 0; i < Player::start_number_of_short_routes; i++) {
        result[i] = short_routes.back();
        short_routes.pop_back();
    }
    for (int i = 0; i < Player::start_number_of_long_routes; i++) {
        result[i + Player::start_number_of_short_routes] = long_routes.back();
        long_routes.pop_back();
    }
    return result;
}
