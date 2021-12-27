#include "PathFindingAlgorithm.h"

#include "Grid.h"
#include <cassert>
#include <cmath>
#include <iostream>
#include <queue>
#include <unordered_map>

PathFindResult a_star_pathfind(const Grid& grid, const sf::Vector2i& start,
                               const sf::Vector2i& finish, int num_neighbours, int pow)
{
    std::priority_queue<Node, std::vector<Node>, NodeCompare> queue;
    std::unordered_map<sf::Vector2i, sf::Vector2i, HashVec2> came_from;
    std::unordered_map<sf::Vector2i, double, HashVec2> cost_so_far;

    cost_so_far[start] = 0;

    // For visualisation
    PathFindResult result;
    result.visited.push_back(start);

    queue.push({start, 0});

    bool found = false;

    while (!queue.empty() and !found) {
        auto current = queue.top();
        queue.pop();
        if (current.pos == finish) {
            found = true;
            break;
        }
        for (int i = 0; i < num_neighbours; i++) {
            auto next = current.pos + sf::Vector2i(X_OFFSET[i], Y_OFFSET[i]);
            double cost = cost_so_far[current.pos] + 1.0;
            if ((try_find(cost_so_far, next) && cost < cost_so_far[next]) ||
                (!try_find(came_from, next) && grid.square_walkable(next))) {
                cost_so_far[next] = cost;

                if (!try_find(came_from, next)) {
                    result.visited.push_back(next);
                }
                double h = heuristic(next, finish);
                h *= (1.0 + 1.0 / 1000.0);
                queue.push({next, cost + std::pow(h, pow)});
                came_from[next] = current.pos;
            }
            if (next == finish) {
                found = true;
                break;
            }
        }
    }

    if (!found) {
        return result;
    }

    result.path = make_path(start, finish, came_from);
    return result;
}