#ifndef TTR_TTRCONTROLLER_H
#define TTR_TTRCONTROLLER_H
#include "Game.h"
#include "Turn.h"

class TTRController {
    // bool is_server_ = false;
    Game *game = nullptr;

    std::string data;

public:
    explicit TTRController() = default;
    void get_card_from_deck();
    void start_game(int num);
    void get_card_from_active(int);
    void build_path_initialize(int);
    void get_routes();
    // void end_game();
    // void make_turn(std::stringstream s);
};

#endif  // TTR_TTRCONTROLLER_H