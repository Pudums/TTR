//
// Created by timofey on 24.02.2021.
//

#include "Algo.h"
bool Algo::is_route_exists(const std::string& s, const std::string &t) {
    std::set<std::string> used;
    dfs(s, t, used);
    if (used.find(t) != used.end()) {
        return true;
    }
    return false;
}

void Algo::dfs(const std::string& current, const std::string &t, std::set<std::string>& used) {
    used.insert(current);
    for (const auto& elem : g[current]) {
        dfs(elem, t, used);
    }
}

Algo::Algo(const std::vector<Path> &paths, const int &player, const std::set<int> &station_paths) {
    for (int i = 0; i < paths.size(); i++) {
        if (paths[i].owner == player || station_paths.find(i) != station_paths.end()) {
            g[paths[i].start].push_back(paths[i].finish);
            g[paths[i].finish].push_back(paths[i].start);
        }
    }
}
