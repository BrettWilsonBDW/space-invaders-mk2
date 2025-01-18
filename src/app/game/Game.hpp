#pragma once

#include "SDL3/SDL.h"

#include "../../levels/Levels.hpp"

class Game
{
public:
    Game(SDL_Window *window, SDL_Renderer *renderer);
    ~Game();

    void Init(SDL_Window *window, SDL_Renderer *renderer);

    void OnInput(SDL_Event *event);
    void OnUpdate();
    void OnRender(float alpha);
private:
    SDL_Window *m_window = nullptr;
    SDL_Renderer *m_renderer = nullptr;

    std::unique_ptr<Levels> m_levels;
};


