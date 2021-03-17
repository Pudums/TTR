//
// Created by timofey on 20.02.2021.
//

#ifndef PROJECT_DECK_H
#define PROJECT_DECK_H

#include <algorithm>
#include <fstream>
#include <iostream>
#include <vector>
#include "Discharge.h"
#include "Player.h"
#include "Route.h"
#include "WagonCard.h"

struct Deck {
    std::vector<WagonCard> wagons_deck;
    std::vector<Route> short_routes;
    std::vector<Route> long_routes;
    void set_start_active_wagons();
    Discharge &discharge;
    WagonCard last_card;

    static const int number_of_active_cards = 5;
    static const int number_of_getting_new_routes = 3;
    static const int max_number_of_active_locomotives = 2;
    static const int number_of_extra_wagons_for_tunnel = 3;
    std::vector<WagonCard> active_wagons;
    Deck(const std::string &wagons_file_name,
         const std::string &short_routes_file_name,
         const std::string &long_routes_file_name,
         Discharge &discharge_);

    void check_correctness_of_deck();
    void replace_active_cards();
    void return_cards_from_discharge();
    [[nodiscard]] bool check_deck_empty();
    [[nodiscard]] bool check_active_card_set_is_correct();

    [[nodiscard]] WagonCard draw_card_from_deck();
    [[nodiscard]] WagonCard draw_card_from_active_cards(int card_number);
    [[nodiscard]] std::vector<WagonCard> get_start_wagon_cards();
    [[nodiscard]] std::vector<Route> get_start_route_cards();
    [[nodiscard]] std::vector<Route> get_new_routes();
    [[nodiscard]] std::vector<WagonCard> get_cards_for_tunnel();
};

#endif  // PROJECT_DECK_H
