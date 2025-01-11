#include "imgui.h"
#include "imgui-SFML.h"
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include <time.h>
#include <iostream>
#include <chrono>

#include "Game.h"

int main()
{
    Game GameLogic;
    GameLogic.randomizeBoard();

    sf::RenderWindow window(sf::VideoMode(GameLogic.CELL_SIZE * (GameLogic.GRID_WIDTH + 6), GameLogic.CELL_SIZE * GameLogic.GRID_HEIGHT), "Game of Life", sf::Style::Titlebar | sf::Style::Close);
    window.setFramerateLimit(100);

    ImGui::SFML::Init(window);
    sf::Clock deltaClock;
    
    while (window.isOpen())
    {
        if (GameLogic.isPlaying)
        {
            GameLogic.frame_counter++;
            GameLogic.frame_general++;

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
                    GameLogic.isPlaying = !GameLogic.isPlaying;
                    GameLogic.frame_counter = 0;
                }

                else if (event.key.code == sf::Keyboard::Escape)
                    window.close();
                    break;
                break;

            case sf::Event::MouseButtonPressed:
                if (!GameLogic.isPlaying && event.mouseButton.button == sf::Mouse::Left)
                {
                    int x = double(event.mouseButton.x) / GameLogic.CELL_SIZE;
                    int y = double(event.mouseButton.y) / GameLogic.CELL_SIZE;
                    if (x >= 0 && x < GameLogic.GRID_WIDTH && y >= 0 && y < GameLogic.GRID_HEIGHT)
                        GameLogic.grid[x + y * GameLogic.GRID_WIDTH] = !GameLogic.grid[x + y * GameLogic.GRID_WIDTH];
                }
                break;
            }
        }

        ImGui::SFML::Update(window, deltaClock.restart());


        GameLogic.showGUI();

        window.clear(WHITE);

        GameLogic.gridLogic(window);

        ImGui::SFML::Render(window);
        window.display();
    }

    ImGui::SFML::Shutdown();
}