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
    int owner = 0;
    while (list_of_paths >> start) {
        list_of_paths >> finish >> color >> number_of_colored_wagons >>
            number_of_locomotives >> is_tunnel;
        paths.push_back({start, finish, color, number_of_colored_wagons,
                         number_of_locomotives, is_tunnel, owner});
    }
    return paths;
}
}  // namespace

Board::Board(const std::string &paths_file_name) {
    std::ifstream list_of_paths(paths_file_name);
    paths = parse_paths_file(list_of_paths);
    for (const auto &elem : paths) {
        std::cout << elem.start << ' ' << elem.finish << ' ' << elem.color
                  << ' ' << elem.number_of_colored_wagons << ' '
                  << elem.number_of_locomotives << ' ' << elem.is_tunnel << ' '
                  << elem.owner << '\n';
    }
}
