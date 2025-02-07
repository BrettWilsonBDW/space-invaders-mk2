#include "Enemy.hpp"

#include <iostream>

Enemy::Enemy(SDL_Window *window, SDL_Renderer *renderer) : Entities(window, renderer)
{
    for (int i = 0; i < 1; i++)
    {
        m_bullets.push_back(Bullet(window, renderer));
        m_bullets[i].SwapDirection();
    }
}

Enemy::~Enemy()
{
}

void Enemy::ShootBullets()
{
    if (m_fired)
    {
        m_fired = false;
        for (auto &bullet : m_bullets)
        {
            bullet.SetActive(true);
            m_bullets[0].SetPosition({GetRect().x + GetRect().w / 2, GetRect().y});
        }
    }

    for (auto &bullet : m_bullets)
    {
        if (!bullet.GetActive())
        {
            bullet.SetPosition({GetRect().x + GetRect().w / 2, GetRect().y});
        }

        if (bullet.CheckCollisions(bullet.GetRect(), m_player->GetRect()))
        {
            bullet.SetActive(false);
            firedOnce = false;

            m_player->SetStatus(true);
        }

        if (bullet.GetRect().y > GetWindowHeight())
        {
            bullet.SetActive(false);
            firedOnce = false;
        }
    }
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

void Enemy::OnUpdate(float dt)
{
    Entities::OnUpdate(dt);

    for (auto &bullet : m_bullets)
    {
        bullet.OnUpdate(dt);
    }

    ShootBullets();

    if (!GetActive())
    {
        SetPosition({0, static_cast<float>(-GetWindowHeight())});
        SetVelocity({0, 0}, 0);
        return;
    }

    MoveThroughTrajectories();
}

void Enemy::OnRender(float alpha)
{
    Entities::OnRender(alpha);

    for (auto &bullet : m_bullets)
    {
        bullet.OnRender(alpha);
    }
}