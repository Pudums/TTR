#include "Game.h"

#ifndef TTR_TTRCONTROLLER_H
#define TTR_TTRCONTROLLER_H

class TTRController {
    //bool is_server_ = false;
    Game &game;
    // View *view;
    // Server *server;
    std::string data;
public:
    explicit TTRController(Game&);
    void set_data();
    void get_data_to_game();
    //TTRController(bool is_server);
    //void start_game();
    //void end_game();
    //void make_turn(std::stringstream s);
};

#endif  // TTR_TTRCONTROLLER_H
