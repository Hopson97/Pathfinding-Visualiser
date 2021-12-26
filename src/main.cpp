#include "Keyboard.h"
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <imgui_sfml/imgui-SFML.h>
#include <imgui_sfml/imgui.h>
#include <numeric>

#include "Grid.h"
#include "PathFindingAlgorithm.h"

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
    // window.setFramerateLimit(60);
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

    PathFindResult path_find_result;

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
                                default:
                                    break;
                            }
                            break;

                        default:
                            break;
                    }
                }
                default:
                    break;
            }
        }

        if (tool == Tool::Wall && sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
            grid.set_tile(sf::Mouse::getPosition(window).x / TILE,
                          sf::Mouse::getPosition(window).y / TILE, State::Blocked);
        }
        else if (sf::Mouse::isButtonPressed(sf::Mouse::Right)) {
            grid.set_tile(sf::Mouse::getPosition(window).x / TILE,
                          sf::Mouse::getPosition(window).y / TILE, State::Empty);
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
                path_find_result = bfs_pathfind(grid, start, finish);
            }

            ImGui::Text("FPS: %.1f", ImGui::GetIO().Framerate);
            ImGui::Text("Current Tool: %s", tool_to_string(tool));

            ImGui::End();
        }

        auto& pfr = path_find_result;
        if (!pfr.visited.empty()) {
            auto current = pfr.visited.front();
            pfr.visited.pop_front();
            grid.set_tile(current.x, current.y, State::Visited);
        }
        else if (!pfr.path.empty()) {
            auto current = pfr.path.back();
            pfr.path.pop_back();
            grid.set_tile(current.x, current.y, State::Path);
        }

        window.clear();

        // Render the grid
        for (int y = 0; y < HEIGHT; y++) {
            for (int x = 0; x < WIDTH; x++) {
                shape.setPosition(x * TILE, y * TILE);
                auto tile = grid.get_tile(x, y);
                switch (tile) {
                    case State::Empty:
                        shape.setFillColor({155, 155, 155});
                        break;
                    case State::Blocked:
                        shape.setFillColor(sf::Color::Black);
                        break;
                    case State::Visited:
                        shape.setFillColor({0, 100, 200});
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