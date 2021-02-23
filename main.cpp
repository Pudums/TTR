#include "include/Board.h"
#include "include/Deck.h"
#include "include/Game.h"

int main() {
    // Deck deck("wagons.txt", "short_routes.txt", "long_routes.txt");
    // Board board("paths.txt");
    Game g(3);
    g.start_game();
    
    return 0;
}
