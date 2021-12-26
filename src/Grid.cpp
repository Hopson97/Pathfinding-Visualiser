#include "Grid.h"

void set_quad_colour(sf::Vertex* vertex_start, State state)
{
    sf::Color colour = sf::Color::White;
    switch (state) {
        case State::Empty:
            colour = sf::Color::White;
            break;
        case State::Blocked:
            colour = sf::Color::Black;
            break;
        case State::Visited:
            colour = {0, 100, 200};
            break;
        case State::Path:
            colour = sf::Color::Green;
            break;
        case State::Start:
            colour = sf::Color::Red;
            break;
        case State::End:
            colour = {255, 165, 25};
            break;
    }

    vertex_start[0].color = colour;
    vertex_start[1].color = colour;
    vertex_start[2].color = colour;
    vertex_start[3].color = colour;
}

Grid::Grid()
{
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            auto vertex_index = (y * WIDTH + x) * 4;
            vertices[vertex_index + 0].position = {x * TILE, y * TILE};
            vertices[vertex_index + 1].position = {(x + 1) * TILE, y * TILE};
            vertices[vertex_index + 2].position = {(x + 1) * TILE, (y + 1) * TILE};
            vertices[vertex_index + 3].position = {x * TILE, (y + 1) * TILE};

            set_quad_colour(&vertices[vertex_index], State::Empty);
        }
    }
}

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

    sf::Vertex* quad = &vertices[(y * WIDTH + x) * 4];
    set_quad_colour(quad, state);

    grid[x + y * WIDTH] = state;
}

void Grid::reset_path_finding()
{
    for (auto& quad : grid) {
        if (quad == State::Path || quad == State::Visited) {
            quad = State::Empty;
        }
    }

    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            set_quad_colour(&vertices[(y * WIDTH + x) * 4], get_tile(x, y));
        }
    }
}

void Grid::draw(sf::RenderWindow& window)
{
    window.draw(vertices.data(), vertices.size(), sf::Quads);
}