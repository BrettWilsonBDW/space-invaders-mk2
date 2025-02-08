#include "SceneManager.hpp"

#include <iostream>

SceneManager::SceneManager(SDL_Window *window, SDL_Renderer *renderer) : m_window(window), m_renderer(renderer)
{
}

SceneManager::~SceneManager()
{
}

void SceneManager::ShowPauseOverlay()
{
    SDL_SetRenderDrawColor(m_renderer, 255, 255, 255, 255);
    SDL_RenderDebugText(m_renderer, (GetWindowWidth() / 2) - 30, GetWindowHeight() / 2, "PAUSED");
}

void SceneManager::ShowGameVictoryOverlay()
{
    SDL_SetRenderDrawColor(m_renderer, 0, 255, 0, 255);
    SDL_RenderDebugText(m_renderer, (GetWindowWidth() / 2) - 50, GetWindowHeight() / 2, "VICTORY");
    SDL_RenderDebugText(m_renderer, (GetWindowWidth() / 2) - 50, (GetWindowHeight() / 2) + 20, "Press R to restart");
}

void SceneManager::ShowGameOverOverlay()
{
    SDL_SetRenderDrawColor(m_renderer, 255, 0, 0, 255);
    SDL_RenderDebugText(m_renderer, (GetWindowWidth() / 2) - 50, GetWindowHeight() / 2, "GAME OVER");
    SDL_RenderDebugText(m_renderer, (GetWindowWidth() / 2) - 50, (GetWindowHeight() / 2) + 20, "Press R to restart");
}

void SceneManager::ShowReadyOverlay()
{
    SDL_SetRenderDrawColor(m_renderer, 255, 255, 255, 255);
    SDL_RenderDebugText(m_renderer, (GetWindowWidth() / 2) - 50, GetWindowHeight() / 2, "READY");
    SDL_RenderDebugText(m_renderer, (GetWindowWidth() / 2) - 50, (GetWindowHeight() / 2) + 20, "Press Enter to start");
}