#include "Game.h"

#ifndef TTR_TTRCONTROLLER_H
#define TTR_TTRCONTROLLER_H

class TTRController {
    TTRController();
    Game *game = nullptr;
    // View *view;
    // Server *server;
    // Network network implement later
public:
    void start_game();
    void end_game();
    void play_game();
};

#endif  // TTR_TTRCONTROLLER_H
