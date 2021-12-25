#include "Keyboard.h"
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <array>
#include <deque>
#include <imgui_sfml/imgui-SFML.h>
#include <imgui_sfml/imgui.h>
#include <iostream>
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
struct HashVec2 {
    size_t operator()(const sf::Vector2i& v) const
    {
        return std::hash<int>()(v.x) ^ std::hash<int>()(v.y);
    }
};

// print sf::vector2i (x, y) to std::cout/std::ostream
std::ostream& operator<<(std::ostream& o, const sf::Vector2i& v)
{
    return o << v.x << ", " << v.y;
}

const int X_OFFSET[] = {-1, 0, 1, 0};
const int Y_OFFSET[] = {0, 1, 0, -1};

std::vector<sf::Vector2i> bfs_pathfind(Grid& grid, const sf::Vector2i& start,
                                       const sf::Vector2i& finish)
{
    std::cout << "Finding a path from " << start << " TO " << finish << std::endl;
    std::deque<sf::Vector2i> queue;
    std::unordered_map<sf::Vector2i, sf::Vector2i, HashVec2> came_from;

    queue.push_back(start);
    grid.set_tile(start.x, start.y, State::Visited);

    bool found = false;

    while (!queue.empty() and !found) {
        auto current = queue.front();
        queue.pop_front();
        for (int i = 0; i < 4; i++) {
            auto next = current + sf::Vector2i(X_OFFSET[i], Y_OFFSET[i]);
            if (came_from.find(next) == came_from.end() &&
                (grid.get_tile(next.x, next.y) == State::Empty ||
                 grid.get_tile(next.x, next.y) == State::End)) {
                grid.set_tile(next.x, next.y, State::Visited);
                queue.push_back(next);
                came_from[next] = current;
                std::cout << "Pushing " << next << " = " << current << "\n";
            }
            if (next == finish) {
                found = true;
                break;
            }
        }
    }

    for (auto& [to, from] : came_from) {
        std::cout << "To " << to << "  From: " << from << "\n";
    }

    auto current = finish;
    std::vector<sf::Vector2i> path;
    while (start != current) {
        std::cout << "Current: " << current << std::endl;
        path.push_back(current);
        current = came_from.at(current);
        grid.set_tile(current.x, current.y, State::Path);
    }
    return path;
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
    sf::Vector2i finish = {-1, -1};

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
                                    if (finish.x != -1) {
                                        grid.set_tile(finish.x, finish.y, State::Empty);
                                    }
                                    grid.set_tile(x, y, State::End);
                                    finish = {x, y};
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

            if (ImGui::Button("Do BFS Path Find")) {
                bfs_pathfind(grid, start, finish);
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