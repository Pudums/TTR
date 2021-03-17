#include "Turn.h"

#include <utility>

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
    num++;
    num %= 2;
}

int BuildPath::get_pos() const {
    return pos;
}

DrawCardFromDeck::DrawCardFromDeck() = default;
void Turn::increase_num() {
    num++;
    num%=2;
}
BuildStation::BuildStation(std::string s) : city(std::move(s)) {
}
std::string BuildStation::get_city() const {
    return city;
}
