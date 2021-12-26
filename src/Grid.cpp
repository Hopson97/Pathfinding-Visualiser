#include "Gridmap.h"

State Grid::get_tile(int x, int y) const
{
    if (x < 0 || x >= WIDTH || y < 0 || y >= HEIGHT) {
        return State::Blocked;
    }
    return grid[x + y * WIDTH];
}

void Grid::set_tile(int x, int y, State state)

{
    if (x < 0 || x >= WIDTH || y < 0 || y >= HEIGHT) {
        return;
    }

    // prevent setting tile if it is a start or end tile being set to visisted
    if (state == State::Visited &&
        (grid[x + y * WIDTH] == State::Start || grid[x + y * WIDTH] == State::End)) {
        return;
    }
    grid[x + y * WIDTH] = state;
}

void Grid::reset_path_finding()
{
    for (auto& quad : grid) {
        if (quad == State::Path || quad == State::Visited) {
            quad = State::Empty;
        }
    }
}