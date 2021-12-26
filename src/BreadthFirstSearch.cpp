#include "PathFindingAlgorithm.h"

#include "Grid.h"
#include <unordered_map>

PathFindResult bfs_pathfind(const Grid& grid, const sf::Vector2i& start,
                            const sf::Vector2i& finish)
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
        for (int i = 0; i < 4; i++) {
            auto next = current + sf::Vector2i(X_OFFSET[i], Y_OFFSET[i]);
            if (came_from.find(next) == came_from.end() &&
                (grid.get_tile(next.x, next.y) == State::Empty ||
                 grid.get_tile(next.x, next.y) == State::End)) {
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

    auto current = finish;
    while (start != current) {
        result.path.push_back(current);
        current = came_from.at(current);
    }
    return result;
}
