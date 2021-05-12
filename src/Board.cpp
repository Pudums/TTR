//
// Created by timofey on 20.02.2021.
//

#include "Board.h"

namespace {
std::vector<Path> parse_paths_file(std::ifstream &list_of_paths) {
    std::vector<Path> paths;
    std::string start;
    std::string finish;
    std::string color;
    int number_of_colored_wagons;
    int number_of_locomotives;
    bool is_tunnel;
    int owner = -1;
    while (list_of_paths >> start) {
        list_of_paths >> finish >> color >> number_of_colored_wagons >>
            number_of_locomotives >> is_tunnel;
        paths.push_back({start, finish, color, number_of_colored_wagons,
                         number_of_locomotives, is_tunnel, owner,
                         number_of_locomotives + number_of_colored_wagons});
    }
    return paths;
}
}  // namespace

Board::Board(const std::string &paths_file_name, const std::string &wagon_blocks_filename) {
    std::ifstream list_of_paths(paths_file_name);
    paths = parse_paths_file(list_of_paths);
    std::ifstream wagon_blocks_filestream(wagon_blocks_filename);
    for (int i = 0; i < paths.size(); i++) {
        paths[i].read_wagon_blocks(wagon_blocks_filestream);
    }
}
