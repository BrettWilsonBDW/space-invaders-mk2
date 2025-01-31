#include "Game.hpp"

Game::Game(SDL_Window *window, SDL_Renderer *renderer) : m_window(window), m_renderer(renderer)
{
}

Game::~Game()
{
}

void Game::Init(SDL_Window *window, SDL_Renderer *renderer)
{
    m_window = window;
    m_renderer = renderer;

    m_levels = std::make_unique<Levels>(m_window, m_renderer);
}

void Game::OnInput(SDL_Event *event)
{
    m_levels->OnInput(event);
}

void Game::OnUpdate(float dt)
{
    m_levels->OnUpdate(dt);
}

void Game::OnRender(float alpha)
{
    m_levels->OnRender(alpha);
}