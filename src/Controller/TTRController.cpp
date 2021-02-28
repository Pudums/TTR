//
// Created by megaserg01 on 25.02.2021.
//

#include "TTRController.h"
TTRController::TTRController(bool is_server) {
    if(is_server) {
        // server = createServer();
    }else{
        //server = createClient();
    }
    int num = 3;  // Server.get_num();
    new (game) Game(num);
}
void TTRController::start_game() {
    game->start_game();
}
void TTRController::end_game() {
    game->end_game();
}
void TTRController::make_turn(std::stringstream s){
    Turn t =nullptr;  //=Parse
        game->make_move(t);
        // view.draw();
    game->end_game();
    //view.draw_end_game();
}
