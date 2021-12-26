#pragma once

#include <array>

constexpr int TILE = 20;
constexpr int WIDTH = 1600 / TILE;
constexpr int HEIGHT = 900 / TILE;

enum class State {
    Empty,
    Blocked,
    Visited,
    Path,
    Start,
    End,
};

struct Grid {
    std::array<State, WIDTH * HEIGHT> grid{State::Empty};

    State get_tile(int x, int y) const;

    void set_tile(int x, int y, State state);

    void reset_path_finding();
};