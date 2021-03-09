//
// Created by timofey on 20.02.2021.
//

#include "Path.h"
int Path::points_for_path(int path_lengths) {
    if (path_lengths == 1) {
        return 1;
    } else if (path_lengths == 2) {
        return 2;
    } else if (path_lengths == 3) {
        return 4;
    } else if (path_lengths == 4) {
        return 7;
    } else if (path_lengths == 6) {
        return 15;
    } else if (path_lengths == 8) {
        return 21;
    }
    return 0;
}
void Path::read_wagon_blocks(std::ifstream &ifs) {
    for (int i = 0; i < length; i++) {
        wagon_blocks.push_back(WagonBlock(ifs));
    }
    WagonBlock::current_id++;
}
