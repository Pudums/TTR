#include "Turn.h"

BuildPath::BuildPath(int id) {
    pos = id;
}

void BuildPath::set_wagons(const std::vector<WagonCard> &new_wagons) {
    for (auto w : new_wagons) {
        cards_to_build.push_back(w);
    }
}

std::vector<WagonCard> BuildPath::getWagons() {
    return cards_to_build;
}

DrawCardFromActive::DrawCardFromActive(std::size_t id) {
    number = id;
}

int BuildPath::get_pos() {
    return pos;
}

DrawCardFromDeck::DrawCardFromDeck() {
    num++;
    num %= 2;
}
