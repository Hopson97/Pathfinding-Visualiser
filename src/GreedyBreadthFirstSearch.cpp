#include "PathFindingAlgorithm.h"

#include "Grid.h"
#include <cmath>
#include <queue>
#include <unordered_map>

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

int heuristic(const sf::Vector2i& start, const sf::Vector2i& end)
{
    int dx = abs(start.x - end.x);
    int dy = abs(start.y - end.y);
    return dx + dy;
}

PathFindResult greedy_bfs_pathfind(const Grid& grid, const sf::Vector2i& start,
                                   const sf::Vector2i& finish)
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
        for (int i = 0; i < 4; i++) {
            auto next = current.pos + sf::Vector2i(X_OFFSET[i], Y_OFFSET[i]);
            if (came_from.find(next) == came_from.end() &&
                (grid.get_tile(next.x, next.y) == State::Empty ||
                 grid.get_tile(next.x, next.y) == State::End)) {
                result.visited.push_back(next);

                int cost = heuristic(next, finish);
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

    auto current = finish;
    while (start != current) {
        result.path.push_back(current);
        current = came_from.at(current);
    }
    return result;
}
