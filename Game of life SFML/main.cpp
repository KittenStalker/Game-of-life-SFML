#include "imgui.h"
#include "imgui-SFML.h"

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include <iostream>
#include <time.h>
#include "Game.h"

#define WHITE sf::Color::White           //dead
#define BLACK sf::Color::Black           //live
#define GRAY sf::Color(153, 153, 153)    

int wrapValue(int v, int vMax)
{
    if (v == -1) return vMax - 1;
    if (v == vMax) return 0;
    return v;
}

int main()
{
    const int CELL_SIZE = 30;
    const int GRID_WIDTH = 30;
    const int GRID_HEIGHT = 20;
    const int N_CELLS = GRID_WIDTH * GRID_HEIGHT;
    const sf::Vector2f CELL_VECTOR(CELL_SIZE, CELL_SIZE);

    const int DELAY_CHANGE = 50;
    int delay = 100;

    int grid[N_CELLS] = {};
    int gridNext[N_CELLS] = {};

    bool isPlaying = true;

    setlocale(LC_ALL, "Russian");
    srand(time(NULL));

    //случ. генерация клеток
    for (int i = 0; i < N_CELLS; i++)
    {
        grid[i] = (rand() % 2) ? 1 : 0;
    }

    //текста
    sf::Font font;
    font.loadFromFile("./arial.ttf");

    sf::Text textPause("Press 'p' to pause.", font);
    textPause.setCharacterSize(15);
    textPause.setPosition(10, CELL_SIZE * GRID_HEIGHT + 5); //todo поменять на норм
    textPause.setFillColor(BLACK);

    sf::Text textPlay("Press 'p' to play.", font);
    textPlay.setCharacterSize(15);
    textPlay.setPosition(10, CELL_SIZE * GRID_HEIGHT + 5);      //todo поменять на норм
    textPlay.setFillColor(BLACK);

    sf::Text textToggle("Click on cell to toggle live/dead cell.", font);
    textToggle.setCharacterSize(15);
    textToggle.setPosition(10, CELL_SIZE * GRID_HEIGHT + 5);           //todo поменять на норм
    textToggle.setFillColor(BLACK);

    sf::Text textSpeed("Use left/right arrow keys to change speed.", font);
    textSpeed.setCharacterSize(15);
    textSpeed.setPosition(300, CELL_SIZE * GRID_HEIGHT + 5);          //todo поменять на норм
    textSpeed.setFillColor(BLACK);



    sf::RenderWindow window(sf::VideoMode(CELL_SIZE * (GRID_WIDTH + 6), CELL_SIZE * GRID_HEIGHT + 50), "Game of Life");  //todo поменять с изменением интерфейса
    window.setFramerateLimit(60);
        
    ImGui::SFML::Init(window);
    sf::Clock deltaClock;

    while (window.isOpen())
    {
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
                    isPlaying = !isPlaying;
                else if (event.key.code == sf::Keyboard::Right)
                    delay = std::max(delay - DELAY_CHANGE, 0); // prevent negative value
                else if (event.key.code == sf::Keyboard::Left)
                    delay += DELAY_CHANGE;
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

        ImGui::Begin("window title");
        ImGui::Text("Text");
        ImGui::End();


        // display grid and prepare gridNext
        window.clear(WHITE);
        for (int x = 0; x < GRID_WIDTH; x++)
        {
            for (int y = 0; y < GRID_HEIGHT; y++)
            {
                // draw cell
                sf::RectangleShape cell;
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
        if (isPlaying)
            for (int i = 0; i < N_CELLS; i++)
                grid[i] = gridNext[i];

        //текст
        window.draw(textSpeed);
        if (isPlaying)
            window.draw(textPause);
        else
        {
            window.draw(textPlay);
            window.draw(textToggle);
        }

        window.display();
        sf::sleep(sf::milliseconds(delay));

    }
    ImGui::SFML::Shutdown();
}