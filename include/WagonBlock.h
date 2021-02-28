//
// Created by timofey on 21.02.2021.
//

#ifndef TTR_WAGONBLOCK_H
#define TTR_WAGONBLOCK_H
#include <fstream>
#include <iostream>
struct Point {
    int x, y;
};

struct Rectangle {
    std::vector<Point> points;
};

struct WagonBlock {
    static inline int current_id = 0;
    std::string color;
    Rectangle coords;
    int id;
    explicit WagonBlock(std::ifstream &wagon_blocks_info);
    // drawing
};

#endif  // TTR_WAGONBLOCK_H
