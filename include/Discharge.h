//
// Created by timofey on 20.02.2021.
//

#ifndef PROJECT_DISCHARGE_H
#define PROJECT_DISCHARGE_H

#include <iostream>
#include <vector>
#include "Route.h"
#include "WagonCard.h"

class Discharge {
    std::vector<WagonCard> deck;
    std::vector<Route> routes;
};

#endif  // PROJECT_DISCHARGE_H
