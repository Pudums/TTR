//
// Created by timofey on 24.02.2021.
//

#ifndef TTR_ALGO_H
#define TTR_ALGO_H
#include <iostream>
#include <map>
#include <set>
#include <string>
#include <vector>
#include "Path.h"

struct Algo {
    std::map<std::string, std::vector<std::string>> g;

    Algo() = default;
    Algo(const std::vector<Path>& paths, const int& player, const std::set<int> &station_paths);

    bool is_route_exists(const std::string& s, const std::string &t);
    void dfs(const std::string& current, const std::string &t, std::set<std::string>& used);
};

#endif  // TTR_ALGO_H
