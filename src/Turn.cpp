#include "Turn.h"

BuildPath::BuildPath(const Path &p) {
    path = p;
}
void BuildPath::set_wagons(std::vector<WagonCard> new_wagons) {
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
void BuildPath::setPath(int n) {
    pos = n;
}
int BuildPath::get_pos() {
    return pos;
}
