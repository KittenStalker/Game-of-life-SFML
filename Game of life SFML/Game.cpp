#include "Game.h"

Game::Game()
{
}

int Game::wrapValue(int v, int vMax)
{
    if (v == -1) return vMax - 1;
    if (v == vMax) return 0;
    return v;
}

void Game::clearBoard() {
    for (int x = 0; x < GRID_WIDTH; x++)
        for (int y = 0; y < GRID_HEIGHT; y++)
        {
            grid[x + y * GRID_WIDTH] = 0;
        }

    frame_counter = 0;
    frame_general = 0;
    move = 0;
}

void Game::randomizeBoard()
{
    srand(time(NULL));
    for (int i = 0; i < N_CELLS; i++)
    {
        grid[i] = (rand() % 2) ? 1 : 0;
    }
}

void Game::showGUI()
{
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
        clearBoard();
    }

    if (ImGui::Button("Random")) {
        clearBoard();
        randomizeBoard();
    }

    ImGui::Text("Delay: %d", delay);
    ImGui::SliderInt("Delay", &delay, DELAY_MIN, DELAY_MAX);

    ImGui::Text("Press SPACE to pause");
    ImGui::Text("Press ESCAPE to close");

    ImGui::End();
}

void Game::draw(sf::RenderWindow& win) {
    win.draw(cell);
}

void Game::gridLogic(sf::RenderTarget& window)
{
    const sf::Vector2f CELL_VECTOR(CELL_SIZE, CELL_SIZE);


    for (int x = 0; x < GRID_WIDTH; x++)
    {
        for (int y = 0; y < GRID_HEIGHT; y++)
        {
            cell.setPosition(x * CELL_SIZE, y * CELL_SIZE);
            cell.setSize(CELL_VECTOR);
            cell.setOutlineThickness(1);
            cell.setOutlineColor(GRAY);
            if (grid[x + y * GRID_WIDTH] == 1)
                cell.setFillColor(BLACK);
            else
                cell.setFillColor(WHITE);

            window.draw(cell);

            //window.draw(cell);

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

    if (isPlaying && (frame_counter % delay == 0)) {
        move += 1;
        for (int i = 0; i < N_CELLS; i++)
            grid[i] = gridNext[i];
    }
}

Game::~Game()
{
}