#pragma once
#include "imgui.h"
#include "imgui-SFML.h"
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include <iostream>
#include <time.h>

#define WHITE sf::Color::White           //dead
#define BLACK sf::Color::Black           //live
#define GRAY sf::Color(153, 153, 153)

class Game
{
private:
    const int DELAY_MIN = 1;
    const int DELAY_MAX = 100;

public:
    const int CELL_SIZE = 30;
    static const int GRID_WIDTH = 30;
    static const int GRID_HEIGHT = 20;

    static const int N_CELLS = GRID_WIDTH * GRID_HEIGHT;
    int grid[N_CELLS] = {};
    int gridNext[N_CELLS] = {};

    int frame_counter = 0;
    int frame_general = 0;
    int move = 0;
    int delay = 20;

    bool isPlaying = true;

    Game();

    void clearBoard();
    void randomizeBoard();
    int wrapValue(int v, int vMax);

    sf::RectangleShape cell;
    void draw(sf::RenderWindow& win);

    void gridLogic(sf::RenderTarget& window);

    void showGUI();



    ~Game();

};


