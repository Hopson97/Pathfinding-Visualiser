#include "PathFindingAlgorithm.h"

#include "Grid.h"
#include <cmath>
#include <queue>
#include <unordered_map>

PathFindResult greedy_bfs_pathfind(const Grid& grid, const sf::Vector2i& start,
                                   const sf::Vector2i& finish, int num_neighbours)
{
    std::priority_queue<Node, std::vector<Node>, NodeCompare> queue;
    std::unordered_map<sf::Vector2i, sf::Vector2i, HashVec2> came_from;

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
            if (!try_find(came_from, next) && grid.square_walkable(next)) {
                result.visited.push_back(next);

                double cost = heuristic(next, finish);
                queue.push({next, cost});
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
