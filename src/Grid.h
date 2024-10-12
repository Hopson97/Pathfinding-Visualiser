#pragma once

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Vertex.hpp>
#include <array>
#include <vector>

constexpr int WIN_WIDTH = 1600;
constexpr int WIN_HEIGHT = 900;

constexpr int TILE = 10;
constexpr int WIDTH = WIN_WIDTH / TILE;
constexpr int HEIGHT = WIN_HEIGHT / TILE;

enum class State {
    Empty,
    Blocked,
    Expensive,
    Visited,
    Path,
    Start,
    End,
};

struct Grid {
    std::vector<State> grid{State::Empty};
    std::vector<sf::Vertex> vertices;
    std::vector<sf::Vertex> grid_lines;

    Grid();
    void draw(sf::RenderWindow& window);

    State get_tile(int x, int y) const;

    void set_tile(int x, int y, State state);

    void reset_path_finding();

    bool square_walkable(const sf::Vector2i& pos) const;
    int cost(const sf::Vector2i& pos) const;

    void generate_random_map(int obstacle_chance);

    void clear_grid();

    void load_grid();
    void save_grid();
};