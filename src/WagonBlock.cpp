//
// Created by timofey on 21.02.2021.
//

#include "WagonBlock.h"
WagonBlock::WagonBlock(std::ifstream &wagon_blocks_info) {
    wagon_blocks_info >> color;
    for (int i = 0; i < 4; i++) {
        int x, y;
        wagon_blocks_info >> x >> y;
        coords.points.push_back(Point{x, y});
    }
    id = current_id;
}
