#pragma once

#include "SDL3/SDL.h"

class SceneManager
{
public:
    SceneManager(SDL_Window *window, SDL_Renderer *renderer);
    ~SceneManager();

    void ShowPauseOverlay();
    void ShowGameVictoryOverlay();
    void ShowGameOverOverlay();
    void ShowReadyOverlay();

private:
    SDL_Window *m_window = nullptr;
    SDL_Renderer *m_renderer = nullptr;

    int GetWindowWidth()
    {
        int width;
        SDL_GetWindowSize(m_window, &width, nullptr);
        return width;
    }
    int GetWindowHeight()
    {
        int height;
        SDL_GetWindowSize(m_window, nullptr, &height);
        return height;
    }
};
