#pragma once

#include <SFML/System/Vector2.hpp>
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

PathFindResult bfs_pathfind(const Grid& grid, const sf::Vector2i& start,
                            const sf::Vector2i& finish);

PathFindResult dijkstra_pathfind(const Grid& grid, const sf::Vector2i& start,
                                 const sf::Vector2i& finish);