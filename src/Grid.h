#pragma once

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Vertex.hpp>
#include <array>
#include <vector>

constexpr int WIN_WIDTH = 1600;
constexpr int WIN_HEIGHT = 900;

constexpr int TILE = 15;
constexpr int WIDTH = WIN_WIDTH / TILE;
constexpr int HEIGHT = WIN_HEIGHT / TILE;

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
    std::array<sf::Vertex, WIDTH * HEIGHT * 4> vertices;
    std::vector<sf::Vertex> grid_lines;

    Grid();
    void draw(sf::RenderWindow& window);

    State get_tile(int x, int y) const;

    void set_tile(int x, int y, State state);

    void reset_path_finding();

    bool square_walkable(const sf::Vector2i& pos) const;
};