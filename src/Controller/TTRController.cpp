//
// Created by megaserg01 on 25.02.2021.
//

#include "TTRController.h"
TTRController::TTRController() {
    // server = createServer();
    int num = 3;  // Server.get_num();
    new (game) Game(num);
    // new (view) View(*game);
}
void TTRController::start_game() {
    // View.start_game();
    game->start_game();
    // server.start();
}
void TTRController::end_game() {
    game->end_game();
}
void TTRController::play_game() {
    while (!game->check_end_game()) {
        Turn *t= nullptr;  //=Server.get_turn();
        game->make_move(t);
        // view.draw();
    }
    game->end_game();
    //view.draw_end_game();
}
