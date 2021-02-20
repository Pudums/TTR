#include "include/Board.h"
#include "include/Deck.h"

int main() {
    Deck deck("wagons.txt", "short_routes.txt", "long_routes.txt");
    Board board("paths.txt");
    
    return 0;
}
