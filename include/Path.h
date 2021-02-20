//
// Created by timofey on 20.02.2021.
//

#ifndef PROJECT_PATH_H
#define PROJECT_PATH_H

#include <iostream>
#include <string>
#include <vector>
#include "Wagon_card.h"

struct Path {
    std::string start;
    std::string finish;
    std::string color;
    int number_of_colored_wagons;
    int number_of_locomotives;
    bool is_tunnel;
    int owner;
    // drawing
};

#endif  // PROJECT_PATH_H
