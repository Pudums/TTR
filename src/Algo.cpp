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

Algo::Algo(const std::vector<Path> &paths, const int &player) {
    for (const auto& path : paths) {
        if (path.owner == player) {
            g[path.start].push_back(path.finish);
            g[path.finish].push_back(path.start);
        }
    }
}
