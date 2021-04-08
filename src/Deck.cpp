//
// Created by timofey on 20.02.2021.
//

#include "Deck.h"
#include <random>

namespace {
std::vector<WagonCard> parse_wagons_file(std::ifstream &list_of_wagons) {
    std::vector<WagonCard> wagons_list;
    std::string color;
    int number_of_wagons;
    while (list_of_wagons >> color) {
        list_of_wagons >> number_of_wagons;
        for (int i = 0; i < number_of_wagons; i++) {
            wagons_list.emplace_back(color);
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
           const std::string &long_routes_file_name,
           Discharge &discharge_)
    : discharge(discharge_) {
    std::ifstream list_of_wagons(wagons_file_name);
    std::ifstream list_of_short_routes(short_routes_file_name);
    std::ifstream list_of_long_routes(long_routes_file_name);
    wagons_deck = parse_wagons_file(list_of_wagons);
    set_start_active_wagons();
    short_routes = parse_routes_file(list_of_short_routes);
    long_routes = parse_routes_file(list_of_long_routes);
}

void Deck::set_start_active_wagons() {
    active_wagons = std::vector<WagonCard>(Deck::number_of_active_cards);
    std::shuffle(wagons_deck.begin(), wagons_deck.end(), std::mt19937(std::random_device()()));
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

std::vector<Route> Deck::get_new_routes() {
    std::vector<Route> result(Deck::number_of_getting_new_routes);
    for (int i = 0; i < Deck::number_of_getting_new_routes; i++) {
        result[i] = short_routes.back();
        short_routes.pop_back();
    }
    return result;
}

bool Deck::check_active_card_set_is_correct() {
    int number_of_locomotives = 0;
    for (const auto &elem : active_wagons) {
        if (elem.color == Multicolored) {
            number_of_locomotives++;
        }
    }
    if (number_of_locomotives > Deck::max_number_of_active_locomotives) {
        return false;
    }
    return true;
}

void Deck::replace_active_cards() {
    while (!active_wagons.empty()) {
        wagons_deck.push_back(active_wagons.back());
        active_wagons.pop_back();
    }
    set_start_active_wagons();
}

bool Deck::check_deck_empty() {
    return wagons_deck.empty();
}

void Deck::return_cards_from_discharge() {
    std::shuffle(discharge.deck.begin(), discharge.deck.end(),
                 std::mt19937(std::random_device()()));
    while (!discharge.deck.empty()) {
        wagons_deck.push_back(discharge.deck.back());
        discharge.deck.pop_back();
    }
}

WagonCard Deck::draw_card_from_deck() {
    WagonCard result = wagons_deck.back();
    wagons_deck.pop_back();
    check_correctness_of_deck();
    last_card = result;
    return result;
}

WagonCard Deck::draw_card_from_active_cards(int card_number) {
    //std::cout << "draw_card ";
    //std::cout.flush();
    WagonCard result = active_wagons[card_number];
    active_wagons[card_number] = wagons_deck.back();
    wagons_deck.pop_back();
    //std::cout << "check_deck_correctness ";
    //std::cout.flush();
    check_correctness_of_deck();
    //std::cout << "correctness_checked ";
    //std::cout.flush();
    last_card = active_wagons[card_number];
    //std::cout << "card_drawn ";
    //std::cout.flush();
    return result;
}

void Deck::check_correctness_of_deck() {
    if (check_deck_empty()) {
        return_cards_from_discharge();
    }
    while (!check_active_card_set_is_correct()) {
        replace_active_cards();
    }
}

std::vector<WagonCard> Deck::get_cards_for_tunnel() {
    std::vector<WagonCard> result(number_of_extra_wagons_for_tunnel);
    for (int i = 0; i < number_of_extra_wagons_for_tunnel; i++) {
        result.push_back(draw_card_from_deck());
    }
    return result;
}
