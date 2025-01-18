#include "Enemy.hpp"

#include <iostream>

Enemy::Enemy(SDL_Window *window, SDL_Renderer *renderer) : Entities(window, renderer)
{
    int width, height;
    SDL_GetWindowSize(window, &width, &height);
}

Enemy::~Enemy()
{
}

void Enemy::MoveThroughTrajectories()
{
    if (m_trajectory.empty())
    {
        return;
    }

    float scaleFactor = GetScaleFactor();

    auto &target = m_trajectory[m_trajectoryIndex];
    float scaledTargetX = target.x * scaleFactor;
    float scaledTargetY = target.y * scaleFactor;

    float dx = scaledTargetX - GetRect().x;
    float dy = scaledTargetY - GetRect().y;

    if (std::hypot(dx, dy) < 10.0f)
    {
        SetPosition({target.x, target.y});
        m_trajectoryIndex++;
        if (m_trajectoryIndex >= m_trajectory.size())
        {
            m_trajectoryIndex = 0;
        }
    }

    auto &nextTarget = m_trajectory[m_trajectoryIndex];
    float scaledNextTargetX = nextTarget.x * scaleFactor;
    float scaledNextTargetY = nextTarget.y * scaleFactor;

    dx = scaledNextTargetX - GetRect().x;
    dy = scaledNextTargetY - GetRect().y;

    float length = std::hypot(dx, dy);
    if (length > 0.0f)
    {
        SetVelocity({(dx), (dy)}, m_speed);
    }
}

void Enemy::OnUpdate()
{
    Entities::OnUpdate();

    MoveThroughTrajectories();
}