#pragma once

#include <SFML/System/Vector2.hpp>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <deque>
#include <unordered_map>

struct Grid;

const int X_OFFSET[] = {-1, 0, 1, 0};
const int Y_OFFSET[] = {0, 1, 0, -1};

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

PathFindResult bfs_pathfind(const Grid& grid, const sf::Vector2i& start,
                            const sf::Vector2i& finish);

PathFindResult dijkstra_pathfind(const Grid& grid, const sf::Vector2i& start,
                                 const sf::Vector2i& finish);

PathFindResult greedy_bfs_pathfind(const Grid& grid, const sf::Vector2i& start,
                                   const sf::Vector2i& finish);

PathFindResult a_star_pathfind(const Grid& grid, const sf::Vector2i& start,
                               const sf::Vector2i& finish);