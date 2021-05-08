//
// Created by timofey on 21.02.2021.
//

#ifndef TTR_WAGONBLOCK_H
#define TTR_WAGONBLOCK_H
#include <fstream>
#include <iostream>
#include <vector>
struct Point {
    int x, y;
};

struct Rectangle {
    std::vector<Point> points;
};
struct Circle{
    Point p;
    int r;
};

struct WagonBlock {
    static inline int current_id = 0;
    std::string color;
    Rectangle coords;
    int id=0;
    explicit WagonBlock(std::ifstream &wagon_blocks_info);
    WagonBlock() = default;
    // drawing
};

#endif  // TTR_WAGONBLOCK_H
