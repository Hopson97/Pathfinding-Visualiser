#include "PathFindingAlgorithm.h"

#include "Grid.h"
#include <unordered_map>

PathFindResult bfs_pathfind(const Grid& grid, const sf::Vector2i& start,
                            const sf::Vector2i& finish, int num_neighbours)
{
    std::deque<sf::Vector2i> queue;
    std::unordered_map<sf::Vector2i, sf::Vector2i, HashVec2> came_from;

    // For visualisation
    PathFindResult result;
    result.visited.push_back(start);

    queue.push_back(start);

    bool found = false;

    while (!queue.empty() and !found) {
        auto current = queue.front();
        queue.pop_front();
        if (current == finish) {
            found = true;
            break;
        }
        for (int i = 0; i < num_neighbours; i++) {
            auto next = current + sf::Vector2i(X_OFFSET[i], Y_OFFSET[i]);
            if (!try_find(came_from, next) && grid.square_walkable(next)) {
                result.visited.push_back(next);
                queue.push_back(next);
                came_from[next] = current;
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
