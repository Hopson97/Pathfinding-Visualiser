#include "PathFindingAlgorithm.h"

#include "Grid.h"
#include <cmath>
#include <queue>
#include <unordered_map>

PathFindResult a_star_pathfind(const Grid& grid, const sf::Vector2i& start,
                               const sf::Vector2i& finish, int num_neighbours)
{
    std::priority_queue<Node, std::vector<Node>, NodeCompare> queue;
    std::unordered_map<sf::Vector2i, sf::Vector2i, HashVec2> came_from;
    std::unordered_map<sf::Vector2i, int, HashVec2> cost_so_far;

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
            int cost = cost_so_far[current.pos] + 1;
            if (((cost_so_far.find(next) != cost_so_far.end() &&
                  cost < cost_so_far[next]) ||
                 (came_from.find(next) == came_from.end())) &&
                (grid.get_tile(next.x, next.y) == State::Empty ||
                 grid.get_tile(next.x, next.y) == State::End)) {

                cost_so_far[next] = cost;
                result.visited.push_back(next);
                queue.push({next, cost + heuristic(next, finish)});
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