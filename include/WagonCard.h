//
// Created by timofey vasilevskij on 19.02.2021.
//

#ifndef PROJECT_WAGON_CARD_H
#define PROJECT_WAGON_CARD_H

#include <iostream>
#include <string>

const std::string White = "White";
const std::string Orange = "Orange";
const std::string Green = "Green";
const std::string Red = "Red";
const std::string Black = "Black";
const std::string Blue = "Blue";
const std::string Yellow = "Yellow";
const std::string Purple = "Purple";
const std::string Multicolored = "Multicolored";
const std::string Uncolored = "Uncolored";
const std::string None = "None";

struct WagonCard {
    std::string color;
    WagonCard();
    WagonCard(std::string color_);
};

#endif  // PROJECT_WAGON_CARD_H
