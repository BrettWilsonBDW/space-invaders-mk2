#pragma once

#include "SDL3/SDL.h"

#include "../../levels/LevelLoader.hpp"

#include "scenes/SceneManager.hpp"

#include "../../utils/Utils.hpp"

class Game
{
public:
    Game(SDL_Window *window, SDL_Renderer *renderer);
    ~Game();

    void Init(SDL_Window *window, SDL_Renderer *renderer);

    void OnInput(SDL_Event *event);
    void OnUpdate(float dt);
    void OnRender(float alpha);
private:
    SDL_Window *m_window = nullptr;
    SDL_Renderer *m_renderer = nullptr;

    Utils* m_utils;

    std::unique_ptr<LevelLoader> m_levelLoader;

    SceneManager m_sceneManager{m_window, m_renderer};

    bool m_isPaused{};

    SDL_Texture *m_backgroundTexture{nullptr};
};


