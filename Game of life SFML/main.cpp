#include "imgui.h"
#include "imgui-SFML.h"
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include <time.h>
#include <iostream>
#include <chrono>

#include "Game.h"


#define WHITE sf::Color::White           //dead
#define BLACK sf::Color::Black           //live
#define GRAY sf::Color(153, 153, 153)    

const int SCALE = 1;

const int CELL_SIZE = 30 / SCALE;
const int GRID_WIDTH = 30 * SCALE;
const int GRID_HEIGHT = 20 * SCALE;

const int N_CELLS = GRID_WIDTH * GRID_HEIGHT;
const sf::Vector2f CELL_VECTOR(CELL_SIZE, CELL_SIZE);

int grid[N_CELLS] = {};
int gridNext[N_CELLS] = {};

int frame_counter = 0;
int frame_general = 0;
int move = 0;

int wrapValue(int v, int vMax)
{
    if (v == -1) return vMax - 1;
    if (v == vMax) return 0;
    return v;
}

void clear_board() {
    for (int x = 0; x < GRID_WIDTH; x++)
        for (int y = 0; y < GRID_HEIGHT; y++)
        {
            grid[x + y * GRID_WIDTH] = 0;
        }

    frame_counter = 0;
    frame_general = 0;
    move = 0;
}

int main()
{
    const int DELAY_MIN = 1;
    const int DELAY_MAX = 100;
    
    int delay = 20;

    bool isPlaying = true;

    setlocale(LC_ALL, "Russian");
    srand(time(NULL));

    //случ. генерация клеток
    for (int i = 0; i < N_CELLS; i++)
    {
        grid[i] = (rand() % 2) ? 1 : 0;
    }


    sf::RenderWindow window(sf::VideoMode(CELL_SIZE * (GRID_WIDTH + 6), CELL_SIZE * GRID_HEIGHT), "Game of Life", sf::Style::Titlebar | sf::Style::Close);
    window.setFramerateLimit(100);
        
    ImGui::SFML::Init(window);
    sf::Clock deltaClock;
    sf::RectangleShape cell;


    while (window.isOpen())
    {
        if (isPlaying)
        {
            frame_counter++;
            frame_general++;

        }
        sf::Event event;
        while (window.pollEvent(event))
        {
            ImGui::SFML::ProcessEvent(event);
            switch (event.type)  //логика на нажатия
            {
            case sf::Event::Closed:
                window.close();
                break;
            case sf::Event::KeyPressed:
                if (event.key.code == sf::Keyboard::Space)
                {
                    isPlaying = !isPlaying;
                    frame_counter = 0;
                }

                else if (event.key.code == sf::Keyboard::Escape)
                    window.close();
                    break;
                break;

            case sf::Event::MouseButtonPressed:
                if (!isPlaying && event.mouseButton.button == sf::Mouse::Left)
                {
                    int x = double(event.mouseButton.x) / CELL_SIZE;
                    int y = double(event.mouseButton.y) / CELL_SIZE;
                    if (x >= 0 && x < GRID_WIDTH && y >= 0 && y < GRID_HEIGHT)
                        grid[x + y * GRID_WIDTH] = !grid[x + y * GRID_WIDTH];
                }
                break;
            }
        }

        ImGui::SFML::Update(window, deltaClock.restart());
        ImGui::Begin("Main menu", 0, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);
        ImGui::SetWindowPos(ImVec2(GRID_WIDTH * CELL_SIZE, 0), ImGuiCond_Always);
        ImGui::SetWindowSize(ImVec2(6 * CELL_SIZE, CELL_SIZE * GRID_HEIGHT + 50), ImGuiCond_Always);

        if (!isPlaying)
            ImGui::Text("Paused!");
        else
            ImGui::Text("Simulation in process");
           
        ImGui::Text("Timer: %d seconds", frame_general / 100);
        ImGui::Text("Move: %d", move);

        if (ImGui::Button("Clear")) {
            clear_board();
            
        }

        if (ImGui::Button("Random")) {
            clear_board();
            for (int i = 0; i < N_CELLS; i++)
            {
                grid[i] = (rand() % 2) ? 1 : 0;
            }
        }
        
        ImGui::Text("Delay: %d", delay);
        ImGui::SliderInt("Delay", &delay, DELAY_MIN, DELAY_MAX);
        
        ImGui::Text("Press SPACE to pause");
        ImGui::Text("Press ESCAPE to close");

        ImGui::End();

        // display grid and prepare gridNext       переделать мб
        window.clear(WHITE);
        for (int x = 0; x < GRID_WIDTH; x++)
        {
            for (int y = 0; y < GRID_HEIGHT; y++)
            {
                // draw cell
                
                cell.setPosition(x * CELL_SIZE, y * CELL_SIZE);
                cell.setSize(CELL_VECTOR);
                cell.setOutlineThickness(1);
                cell.setOutlineColor(GRAY);
                if (grid[x + y * GRID_WIDTH] == 1)
                    cell.setFillColor(BLACK);
                else
                    cell.setFillColor(WHITE);
                window.draw(cell);

                // prepare gridNext
                if (isPlaying)
                {
                    int neighborSum = 0;
                    for (int i = -1; i < 2; i++)
                        for (int j = -1; j < 2; j++)
                        {
                            int xi = wrapValue(x + i, GRID_WIDTH);
                            int yj = wrapValue(y + j, GRID_HEIGHT);
                            neighborSum += grid[xi + yj * GRID_WIDTH];
                        }

                    int current = x + y * GRID_WIDTH;
                    neighborSum -= grid[current];
                    gridNext[current] = grid[current];
                    if (grid[current] == 1 && (neighborSum < 2 || neighborSum > 3))
                        gridNext[current] = 0;
                    else if (neighborSum == 3)
                        gridNext[current] = 1;
                }
            }
            
        }

        ImGui::SFML::Render(window);

        // move gridNext to grid
        if (isPlaying && (frame_counter % delay == 0)){
            move += 1;
            for (int i = 0; i < N_CELLS; i++)
                grid[i] = gridNext[i];
        }


        window.display();

    }
    ImGui::SFML::Shutdown();
}