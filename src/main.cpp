#include "Keyboard.h"
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <array>
#include <imgui_sfml/imgui-SFML.h>
#include <imgui_sfml/imgui.h>
#include <numeric>
#include <queue>

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

enum class Tool {
    Start,
    Finish,
    Wall,
};

const char* tool_to_string(Tool tool)
{
    switch (tool) {
        case Tool::Start:
            return "Start";
        case Tool::Finish:
            return "Finish";
        case Tool::Wall:
            return "Wall";
    }
    return "";
}

int main()
{
    sf::RenderWindow window({1600, 900}, "Pathfinding");
    window.setFramerateLimit(60);
    window.setKeyRepeatEnabled(false);

    ImGui::SFML::Init(window);
    ImGui::GetStyle().FrameRounding = 0;
    ImGui::GetStyle().WindowRounding = 0;

    sf::RectangleShape shape({TILE, TILE});
    shape.setOutlineColor(sf::Color::Black);
    shape.setOutlineThickness(1);

    Tool tool = Tool::Wall;
    sf::Vector2i start = {-1, -1};
    sf::Vector2i end = {-1, -1};

    Keyboard keyboard;

    Grid grid;
    sf::Clock updateClock;
    while (window.isOpen()) {
        sf::Event e;
        while (window.pollEvent(e)) {
            keyboard.update(e);
            ImGui::SFML::ProcessEvent(e);
            switch (e.type) {
                case sf::Event::Closed:
                    window.close();
                    break;

                case sf::Event::MouseButtonReleased: {
                    if (ImGui::GetIO().WantCaptureMouse) {
                        break;
                    }
                    int x = e.mouseButton.x / TILE;
                    int y = e.mouseButton.y / TILE;

                    switch (e.mouseButton.button) {
                        case sf::Mouse::Left:
                            switch (tool) {
                                case Tool::Start:
                                    if (start.x != -1) {
                                        grid.set_tile(start.x, start.y, State::Empty);
                                    }
                                    grid.set_tile(x, y, State::Start);
                                    start = {x, y};
                                    break;
                                case Tool::Finish:
                                    if (end.x != -1) {
                                        grid.set_tile(end.x, end.y, State::Empty);
                                    }
                                    grid.set_tile(x, y, State::End);
                                    end = {x, y};
                                    break;
                                case Tool::Wall:
                                    grid.set_tile(x, y, State::Blocked);
                                    break;
                            }
                            break;

                        case sf::Mouse::Right:
                            grid.set_tile(x, y, State::Empty);
                            break;

                        default:
                            break;
                    }
                }
                default:
                    break;
            }
        }

        ImGui::SFML::Update(window, updateClock.restart());

        if (ImGui::Begin("Menu")) {
            if (ImGui::Button("Clear")) {
                grid.grid.fill(State::Empty);
            }
            if (ImGui::Button("Start")) {
                tool = Tool::Start;
            }
            if (ImGui::Button("Finish")) {
                tool = Tool::Finish;
            }
            if (ImGui::Button("Wall")) {
                tool = Tool::Wall;
            }

            ImGui::Text("FPS: %.1f", ImGui::GetIO().Framerate);
            ImGui::Text("Current Tool: %s", tool_to_string(tool));

            ImGui::End();
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
                        shape.setFillColor({255, 165, 25});
                        break;
                }

                window.draw(shape);
            }
        }

        ImGui::SFML::Render(window);
        window.display();
    }
}