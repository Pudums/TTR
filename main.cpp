#include "TTRController.h"
#include "Game.h"
#include "Test.h"

void play(int argc, char *argv[]) {
    QApplication a(argc, argv);
    View *view = new View();
    view->display_menu();
    a.exec();
}

int main() {
    // Deck deck("wagons.txt", "short_routes.txt", "long_routes.txt");
    // Board board("paths.txt");
    // TTRController controller;
    // controller.start_game(2);
    Test::run_tests();

    return 0;
}
