#include "imgui.h"
#include "imgui-SFML.h"
#include "Keyboard.h"
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
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
    srand(time(0));
    sf::RenderWindow window({WIN_WIDTH, WIN_HEIGHT}, "Pathfinding");
    // window.setFramerateLimit(500);
    window.setKeyRepeatEnabled(false);

    ImGui::SFML::Init(window);
    ImGui::GetStyle().FrameRounding = 0;
    ImGui::GetStyle().WindowRounding = 0;

    Tool tool = Tool::Wall;
    sf::Vector2i start = {-1, -1};
    sf::Vector2i finish = {-1, -1};

    Keyboard keyboard;

    PathFindResult path_find_result;

    int num_neighbours = NEIGHBOURS;

    int nodes_visited = 0;
    int path_length = 0;

    int a_star_pow = 1;

    int map_obstacles = 0;

    Grid grid;
    grid.load_grid();
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
            int x = sf::Mouse::getPosition(window).x / TILE;
            int y = sf::Mouse::getPosition(window).y / TILE;

            for (int yo = 0; yo < 2; yo++) {
                for (int xo = 0; xo < 2; xo++) {
                    grid.set_tile(x + xo, y + yo, State::Blocked);
                }
            }
        }

        else if (sf::Mouse::isButtonPressed(sf::Mouse::Right)) {
            grid.set_tile(sf::Mouse::getPosition(window).x / TILE,
                          sf::Mouse::getPosition(window).y / TILE, State::Empty);
        }

        ImGui::SFML::Update(window, updateClock.restart());

        if (ImGui::Begin("Menu")) {

            ImGui::Text("Tool Select");

            ImGui::SameLine();
            if (ImGui::Button("Start")) {
                tool = Tool::Start;
            }
            ImGui::SameLine();
            if (ImGui::Button("Finish")) {
                tool = Tool::Finish;
            }
            ImGui::SameLine();
            if (ImGui::Button("Wall")) {
                tool = Tool::Wall;
            }

            ImGui::Separator();
            ImGui::Text("Map Options");

            if (ImGui::Button("Clear")) {
                grid.grid.fill(State::Empty);
                grid.reset_path_finding();
            }

            ImGui::SliderInt("Map Obstacles", &map_obstacles, 0, 100);
            if (ImGui::Button("Generate")) {
                grid.generate_random_map(map_obstacles);
            }

            ImGui::Separator();
            ImGui::Text("Pathfinding Algorithm");
            if (ImGui::Button("Breadth First Search")) {
                grid.reset_path_finding();
                nodes_visited = 0;
                path_length = 0;
                path_find_result = bfs_pathfind(grid, start, finish, num_neighbours);
            }
            ImGui::SameLine();
            if (ImGui::Button("Dijkstra's")) {
                grid.reset_path_finding();
                nodes_visited = 0;
                path_length = 0;
                path_find_result = dijkstra_pathfind(grid, start, finish, num_neighbours);
            }
            if (ImGui::Button("Greedy Best First Search")) {
                grid.reset_path_finding();
                nodes_visited = 0;
                path_length = 0;
                path_find_result =
                    greedy_bfs_pathfind(grid, start, finish, num_neighbours);
            }
            ImGui::SameLine();
            if (ImGui::Button("A*")) {
                grid.reset_path_finding();
                nodes_visited = 0;
                path_length = 0;
                path_find_result =
                    a_star_pathfind(grid, start, finish, num_neighbours, a_star_pow);
            }

            ImGui::SliderInt("Neighbours?", &num_neighbours, 4, 8);
            ImGui::SliderInt("A* pow?", &a_star_pow, 1, 10);

            ImGui::Separator();
            ImGui::Text("Pathfinding Results");
            ImGui::Text("Nodes Visited: %d", nodes_visited);
            ImGui::Text("Path Length: %d", path_length);
            ImGui::Text("FPS: %.1f", ImGui::GetIO().Framerate);
            ImGui::Text("Current Tool: %s", tool_to_string(tool));
            ImGui::End();
        }

        auto& pfr = path_find_result;
        if (!pfr.visited.empty()) {
            auto current = pfr.visited.front();
            pfr.visited.pop_front();
            grid.set_tile(current.x, current.y, State::Visited);
            nodes_visited++;
        }
        else if (!pfr.path.empty()) {
            auto current = pfr.path.back();
            pfr.path.pop_back();
            grid.set_tile(current.x, current.y, State::Path);
            path_length++;
        }

        window.clear();

        // Render the grid
        grid.draw(window);
        ImGui::SFML::Render(window);
        window.display();
    }

    grid.save_grid();
}