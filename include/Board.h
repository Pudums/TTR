//
// Created by timofey on 20.02.2021.
//

#ifndef PROJECT_BOARD_H
#define PROJECT_BOARD_H

#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include "Path.h"

struct Board {
    std::vector<Path> paths;
    explicit Board(const std::string &paths_file_name);
};

#endif  // PROJECT_BOARD_H
