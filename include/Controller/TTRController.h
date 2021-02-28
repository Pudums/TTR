#ifndef TTR_TTRCONTROLLER_H
#define TTR_TTRCONTROLLER_H
#include "Game.h"
#include "Turn.h"

class TTRController {
    // bool is_server_ = false;
    Game *game = nullptr;
    Turn *current_turn = nullptr;

public:
    explicit TTRController() = default;
    void get_card_from_deck();
    void start_game(int num);
    void get_card_from_active(int);
    void build_path_initialize(int);
    void set_color_to_build_path(const WagonCard &w);
    void get_routes();
    const std::vector<WagonCard> &get_current_player_cards();
    // void end_game();
    // void make_turn(std::stringstream s);
    ~TTRController();
};

#endif  // TTR_TTRCONTROLLER_H