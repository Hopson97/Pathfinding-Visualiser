#pragma once

#include <SFML/System/Vector2.hpp>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <deque>
#include <unordered_map>

struct Grid;

constexpr int NEIGHBOURS = 4;
const int X_OFFSET[] = {-1, 0, 1, 0, -1, 1, 1, -1};
const int Y_OFFSET[] = {0, 1, 0, -1, 1, 1, -1, -1};

struct HashVec2 {
    size_t operator()(const sf::Vector2i& v) const
    {
        return std::hash<int>()(v.x) ^ std::hash<int>()(v.y);
    }
};

struct PathFindResult {
    std::deque<sf::Vector2i> visited;
    std::deque<sf::Vector2i> path;
};

struct Node {
    sf::Vector2i pos;
    int cost;
};

struct NodeCompare {
    bool operator()(const Node& lhs, const Node& rhs) const
    {
        return lhs.cost > rhs.cost;
    }
};

inline int heuristic(const sf::Vector2i& start, const sf::Vector2i& end)
{
    int dx = abs(start.x - end.x);
    int dy = abs(start.y - end.y);
    return dx + dy;
}

/**
 * @brief Constructs a path from the start to the end by back-tracking through the
 * searched nodes.
 *
 * @param start The start position of the path
 * @param goal The end position of the path
 * @param came_from Map of visited nodes to the previous node
 * @return std::vector<sf::Vector2i> Path from goal to end (as it was back tracked)
 */
inline std::deque<sf::Vector2i>
make_path(const sf::Vector2i& start, const sf::Vector2i& goal,
          std::unordered_map<sf::Vector2i, sf::Vector2i, HashVec2>& came_from)
{
    std::deque<sf::Vector2i> path;
    auto current = goal;
    while (start != current) {
        path.push_back(current);
        current = came_from.at(current);
    }
    return path;
}

template <typename Map, typename Key>
bool try_find(const Map& map, const Key& key)
{
    return map.find(key) != map.end();
}

PathFindResult bfs_pathfind(const Grid& grid, const sf::Vector2i& start,
                            const sf::Vector2i& finish, int neighbour_count);

PathFindResult dijkstra_pathfind(const Grid& grid, const sf::Vector2i& start,
                                 const sf::Vector2i& finish, int neighbour_count);

PathFindResult greedy_bfs_pathfind(const Grid& grid, const sf::Vector2i& start,
                                   const sf::Vector2i& finish, int neighbour_count);

PathFindResult a_star_pathfind(const Grid& grid, const sf::Vector2i& start,
                               const sf::Vector2i& finish, int neighbour_count, int pow);