#include <QApplication>
#include "include/Controller/TTRController.h"
#include "include/Game.h"
#include "include/View/View.h"

void play(int argc, char *argv[]) {
    QApplication a(argc, argv);
    View view = new View();
    view.display_menu();
    a.exec();
}

int main(int argc, char *argv[]) {
    // Deck deck("wagons.txt", "short_routes.txt", "long_routes.txt");
    // Board board("paths.txt");
    play(argc, argv);

    return 0;
}
