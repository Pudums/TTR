//
// Created by timofey on 20.02.2021.
//

#ifndef PROJECT_DISCHARGE_H
#define PROJECT_DISCHARGE_H

#include "Wagon_card.h"
#include "Route.h"
#include <iostream>
#include <vector>

class Discharge {
    std::vector<Wagon_card> deck;
    std::vector<Route> routes;
};

#endif  // PROJECT_DISCHARGE_H
