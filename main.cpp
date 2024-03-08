#include <iostream>
#include <memory>
#include <vector>
#include "primitives.h"

const int WIDTH = 1300, HEIGHT = 1000; // Screen parameters in pixels
const int OFFSET = 12;               // Cell size in pixels
const int DELAY = 20;   //Delay between cycles in ms

void initCells(int offset, std::vector<std::vector<std::unique_ptr<Rect>>> &cells) // creating Rects for all cells of grid
{
    for (int y = 0; y < HEIGHT; y += offset)
    {
        for (int x = 0; x < WIDTH; x += offset)
        {
            std::unique_ptr<Rect> r = std::make_unique<Rect>(Rect(x, y, x + offset, y + offset, false, 0, 0, 0, 255));
            cells[y / offset].push_back(std::move(r));
        }
    }
}

std::vector<std::vector<int>> findNeighbours(int x, int y, int x_max, int y_max) // finding neighbours for cell
{
    std::vector<std::vector<int>> res(8);
    int counter = 0;
    for (int i = -1; i < 2; i++)
    {
        for (int j = -1; j < 2; j++)
        {
            if (i == 0 && j == 0)
                continue;

            if (y + j < 0)
                res[counter].push_back(y_max);
            else if (y + j > y_max)
                res[counter].push_back(0);
            else
                res[counter].push_back(y + j);

            if (x + i < 0)
                res[counter].push_back(x_max);
            else if (x + i > x_max)
                res[counter].push_back(0);
            else
                res[counter].push_back(x + i);

            counter++;
        }
    }
    return res;
}

int main(int argc, char *argv[])
{
    SDL_Init(SDL_INIT_EVERYTHING);

    SDL_Window *window = SDL_CreateWindow("Game of life!", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIDTH, HEIGHT, SDL_WINDOW_ALLOW_HIGHDPI);

    if (NULL == window)
    {
        std::cout << "Could not create window: " << SDL_GetError() << std::endl;
        return 1;
    }

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);

    std::unique_ptr<Grid> g = std::make_unique<Grid>(Grid(OFFSET, 0, 0, 0, 255));
    g->Draw(renderer, HEIGHT, WIDTH);
    std::vector<std::vector<std::unique_ptr<Rect>>> cells(HEIGHT / OFFSET);
    initCells(OFFSET, cells);

    SDL_RenderPresent(renderer);

    SDL_Event windowEvent;

    bool started = false;
    while (!started)
    {

        if (SDL_PollEvent(&windowEvent))
        {
            if (SDL_QUIT == windowEvent.type)
            {
                SDL_DestroyWindow(window);
                SDL_Quit();
                return EXIT_SUCCESS;
            }
            if (SDL_MOUSEBUTTONDOWN == windowEvent.type)
            {
                int x, y;
                if (SDL_BUTTON_LEFT == windowEvent.button.button)
                {
                    SDL_GetMouseState(&x, &y);
                    if (!(x > WIDTH / OFFSET * OFFSET || y > HEIGHT / OFFSET * OFFSET))
                    {
                        cells[y / OFFSET][x / OFFSET]->ChangeColor(255, 255, 255);
                        cells[y / OFFSET][x / OFFSET]->Draw(renderer);
                        cells[y / OFFSET][x / OFFSET]->painted = !cells[y / OFFSET][x / OFFSET]->painted;
                        cells[y / OFFSET][x / OFFSET]->ChangeColor(0, 0, 0);
                        cells[y / OFFSET][x / OFFSET]->Draw(renderer);
                        SDL_RenderPresent(renderer);
                    }
                }
            }
            if (SDL_KEYDOWN == windowEvent.type)
            {
                if (SDLK_f == windowEvent.key.keysym.sym)
                {
                    started = true;
                }
            }
        }
    }
    while (true)
    {
        if (SDL_PollEvent(&windowEvent))
        {
            if (SDL_QUIT == windowEvent.type)
            {
                SDL_DestroyWindow(window);
                SDL_Quit();
                return EXIT_SUCCESS;
            }
        }

        std::vector<std::vector<bool>> tmp(cells.size());
        for (int i = 0; i < cells.size(); i++)
        {
            for (int j = 0; j < cells[i].size(); j++)
            {
                tmp[i].push_back(cells[i][j]->painted);
            }
        }
        for (int i = 0; i < cells.size(); i++)
        {
            for (int j = 0; j < cells[i].size(); j++)
            {
                std::vector<std::vector<int>> neighbours = findNeighbours(j, i, cells[0].size() - 1, cells.size() - 1);
                int activeCount = 0;

                for (int k = 0; k < neighbours.size(); k++)
                    if (tmp[neighbours[k][0]][neighbours[k][1]])
                        activeCount++;
                if (activeCount == 3 || activeCount == 2 && cells[i][j]->painted)
                {
                    cells[i][j]->painted = true;
                    cells[i][j]->Draw(renderer);
                }
                else
                {
                    cells[i][j]->ChangeColor(255, 255, 255);
                    cells[i][j]->Draw(renderer);
                    cells[i][j]->painted = false;
                    cells[i][j]->ChangeColor(0, 0, 0);
                    cells[i][j]->Draw(renderer);
                }
            }
        }
        SDL_RenderPresent(renderer);
        _sleep(DELAY);
    }
    SDL_DestroyWindow(window);
    SDL_Quit();

    return EXIT_SUCCESS;
}