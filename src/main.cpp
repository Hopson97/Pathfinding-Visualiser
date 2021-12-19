#include "Keyboard.h"
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
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

    State get_tile(int x, int y)
    {
        if (x < 0 || x >= WIDTH || y < 0 || y >= HEIGHT) {
            return State::Blocked;
        }
        return grid[x + y * WIDTH];
    }

    void set_tile(int x, int y, State state) { grid[x + y * WIDTH] = state; }
};

int main()
{
    sf::RenderWindow window({1600, 900}, "SFML");
    window.setFramerateLimit(60);
    window.setKeyRepeatEnabled(false);

    sf::RectangleShape shape({TILE, TILE});
    shape.setOutlineColor(sf::Color::Black);
    shape.setOutlineThickness(1);

    Keyboard keyboard;

    Grid grid;

    while (window.isOpen()) {
        sf::Event e;
        while (window.pollEvent(e)) {
            keyboard.update(e);
            switch (e.type) {
                case sf::Event::Closed:
                    window.close();
                    break;

                default:
                    break;
            }
        }

        window.clear();

        // Render the grid
        for (int y = 0; y < HEIGHT; y++) {
            for (int x = 0; x < WIDTH; x++) {
                shape.setPosition(x * TILE, y * TILE);
                auto tile = grid.get_tile(x, y);
                switch (tile) {
                    case State::Empty:
                        shape.setFillColor(sf::Color::White);
                        break;
                    case State::Blocked:
                        shape.setFillColor(sf::Color::Black);
                        break;
                    case State::Visited:
                        shape.setFillColor(sf::Color::Blue);
                        break;
                    case State::Path:
                        shape.setFillColor(sf::Color::Green);
                        break;
                    case State::Start:
                        shape.setFillColor(sf::Color::Red);
                        break;
                    case State::End:
                        shape.setFillColor(sf::Color::Yellow);
                        break;
                }

                window.draw(shape);
            }
        }

        window.display();
    }
}