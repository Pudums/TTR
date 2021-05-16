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
#include "Algo.h"
#include "Board.h"
#include "Deck.h"
#include "Discharge.h"
#include "Player.h"
#include "Turn.h"

struct Game {
private:
    static const int number_of_wagons_for_finish = 20;
    static const int number_of_getting_wagons = 2;

public:
    Board board;
    Discharge discharge;
    Deck deck;
    std::vector<Player> players;
    int active_player;
    int number_of_players;
    std::vector<std::pair<Point, std::string>> cities;
    std::unordered_map<std::string, int> occupied_stations;
    int number_updated_players = 0;
    explicit Game(int number_of_players, int number_of_bots);

    void start_game();
    void make_move(Turn *turn);
    void update_station_path(const std  ::string &station_city, int path_pos);
    void move_get_new_roots();
    std::set<std::string> players_cities();
    [[nodiscard]] bool move_build_path(int position,
                                       const std::vector<WagonCard> &cards);
    bool get_wagon_card_from_deck();
    [[nodiscard]] bool move_build_station(const std::string &city);
    bool get_wagon_card_from_active_cards(int position);
    void add_extra_tunnel_cards(Path &path);
    void update_state_after_path_building(
        Path &path,
        const std::vector<WagonCard> &list_of_wagon_cards);
    [[nodiscard]] int check_end_game() const;
    [[nodiscard]] static std::vector<WagonCard> cards_with_suitable_color(
        const WagonCard &wagon_card, const Player& player) ;
    [[nodiscard]] int number_of_cards_with_fixed_color(
        const std::string &color) const;
    [[nodiscard]] std::map<std::string, int> color_to_num() const;
    [[nodiscard]] bool check_if_enough_cards_for_building_path(
        const Path &path,
        const std::vector<WagonCard> &list_of_cards) const;
    void end_game();
    void count_players_points();
};

#endif  // PROJECT_GAME_H
