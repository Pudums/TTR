//
// Created by timofey on 24.02.2021.
//

#include "Algo.h"
namespace {
struct Edge {
    int length;
    std::string to_city;
    int path_pos;
};
}  // namespace
bool Algo::is_route_exists(const std::string &s, const std::string &t) {
    std::set<std::string> used;
    dfs(s, t, used);
    if (used.find(t) != used.end()) {
        return true;
    }
    return false;
}

void Algo::dfs(const std::string &current,
               const std::string &t,
               std::set<std::string> &used) {
    used.insert(current);
    for (const auto &elem : g[current]) {
        dfs(elem, t, used);
    }
}

Algo::Algo(const std::vector<Path> &paths,
           const int &player,
           const std::set<int> &station_paths) {
    for (int i = 0; i < paths.size(); i++) {
        if (paths[i].owner == player ||
            station_paths.find(i) != station_paths.end()) {
            g[paths[i].start].push_back(paths[i].finish);
            g[paths[i].finish].push_back(paths[i].start);
        }
    }
}

int Algo::find_best_way(const std::string &start,
                        const std::set<std::string> &visited_cities,
                        const std::vector<Path> &all_paths) {
    std::map<std::string, std::pair<int, int>> dist;
    std::set<std::string> used_cities, all_cities;
    std::pair<int, int> shortest_path(INT32_MAX, INT32_MAX);
    std::map<std::string, std::vector<Edge>> g;
    dist[start] = std::make_pair(INT32_MAX, 0);
    for (int i = 0; i < all_paths.size(); i++) {
        if (all_paths[i].owner == -1) {
            g[all_paths[i].start].push_back(
                Edge{all_paths[i].length, all_paths[i].finish, i});
            g[all_paths[i].finish].push_back(
                Edge{all_paths[i].length, all_paths[i].start, i});
            all_cities.insert(all_paths[i].start);
            all_cities.insert(all_paths[i].finish);
        }
    }
    for (int i = 0; i < all_cities.size(); i++) {
        std::string name_of_closest;
        for (const auto &elem : all_cities) {
            if (used_cities.find(elem) == used_cities.end()) {
                if (dist.find(elem) != dist.end() &&
                    (name_of_closest.empty() ||
                     dist[elem].second < dist[name_of_closest].second)) {
                    name_of_closest = elem;
                }
            }
            if (visited_cities.find(name_of_closest) != visited_cities.end() &&
                dist[name_of_closest].first < shortest_path.first) {
                shortest_path = dist[name_of_closest];
            }
            for (const auto &edge : g[name_of_closest]) {
                if (dist.find(edge.to_city) == dist.end() ||
                    dist[edge.to_city].first >
                        dist[name_of_closest].first + edge.length) {
                    dist[edge.to_city] = {
                        dist[name_of_closest].first + edge.length,
                        edge.path_pos};
                }
            }
            used_cities.insert(name_of_closest);
        }
    }
    return shortest_path.second;
}
