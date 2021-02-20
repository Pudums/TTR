//
// Created by timofey on 20.02.2021.
//

#ifndef PROJECT_DECK_H
#define PROJECT_DECK_H

#include "Route.h"
#include "Wagon_card.h"
#include <algorithm>
#include <fstream>
#include <iostream>
#include <vector>

struct Deck {
private:
    std::vector<Wagon_card> wagons_deck;
    std::vector<Wagon_card> active_wagons;
    std::vector<Route> short_routes;
    std::vector<Route> long_routes;
    void set_start_active_wagons();

public:
    static const int number_of_active_cards = 5;
    Deck(const std::string &wagons_file_name,
         const std::string &short_routes_file_name,
         const std::string &long_routes_file_name);

    [[nodiscard]] int wagons_deck_size() const;
    [[nodiscard]] bool is_wagons_deck_empty() const;
    [[nodiscard]] int routes_deck_size() const;
    [[nodiscard]] bool is_routes_deck_empty() const;
};

#endif  // PROJECT_DECK_H
