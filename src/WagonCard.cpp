//
// Created by timofey on 19.02.2021.
//

#include "WagonCard.h"

#include <utility>
WagonCard::WagonCard(std::string s) : color(std::move(s)) {
}
