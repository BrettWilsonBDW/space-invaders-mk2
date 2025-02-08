#include "Game.hpp"

Game::Game(SDL_Window *window, SDL_Renderer *renderer) : m_window(window), m_renderer(renderer), m_utils(&Utils::GetInstance())
{
}

Game::~Game()
{
}

void Game::Init(SDL_Window *window, SDL_Renderer *renderer)
{
    m_window = window;
    m_renderer = renderer;

    m_levelLoader = std::make_unique<LevelLoader>(m_window, m_renderer);
    m_sceneManager = SceneManager(m_window, m_renderer);

    m_backgroundTexture = m_utils->GetTexture("spaceBackground.png");
}

void Game::OnInput(SDL_Event *event)
{
    m_levelLoader->OnInput(event);
}

void Game::OnUpdate(float dt)
{
    if (dt == 0)
    {
        m_isPaused = true;
    }
    else
    {
        m_isPaused = false;
    }

    if (m_levelLoader->GetDisplayingReadyScreen())
    {
        dt = 0;
    }
    
    m_levelLoader->OnUpdate(dt);

    if (m_levelLoader->GetGameOverState() == 1)
    {
        m_sceneManager.ShowGameVictoryOverlay();
    }
}

void Game::OnRender(float alpha)
{
    SDL_FRect destRect;

    destRect.x = 0;
    destRect.y = 0;
    destRect.w = m_utils->GetWindowWidth();
    destRect.h = m_utils->GetWindowHeight();
    SDL_SetRenderDrawColor(m_renderer, 255, 255, 255, 255);
    SDL_RenderTexture(m_renderer, m_backgroundTexture, NULL, &destRect);

    if (m_isPaused)
    {
        m_sceneManager.ShowPauseOverlay();
    }
    else
    {
        m_levelLoader->OnRender(alpha);

        if (m_levelLoader->GetGameOverState() == 1)
        {
            m_sceneManager.ShowGameVictoryOverlay();
        }
    
        if (m_levelLoader->GetGameOverState() == 2)
        {
            m_sceneManager.ShowGameOverOverlay();
        }

        if (m_levelLoader->GetDisplayingReadyScreen() && m_levelLoader->GetGameOverState() != 1)
        {
            m_sceneManager.ShowReadyOverlay();
        }
    }
}