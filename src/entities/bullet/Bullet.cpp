#include "Bullet.hpp"

#include <iostream>

Bullet::Bullet(SDL_Window *window, SDL_Renderer *renderer) : Entities(window, renderer)
{
    SetRect({100, 0, 5, 10});

    SetPosition({(static_cast<float>(GetWindowWidth()) / 2) - 50, static_cast<float>(GetWindowHeight()) - 100});
}

Bullet::~Bullet()
{
}

bool Bullet::CheckCollisions(SDL_FRect a, SDL_FRect b)
{
    return (a.x < b.x + b.w &&
            a.x + a.w > b.x &&
            a.y < b.y + b.h &&
            a.y + a.h > b.y);
}

void Bullet::OnUpdate()
{
    if (!m_active)
    {
        SetVelocity({0, 0}, 0);
    }

    Entities::OnUpdate();

    Vector2 vel = {0, 0};

    float speed{20000};

    speed = speed * (1.0f / 60.0f);

    if (!m_direction)
    {
        vel.y -= speed;
    }
    else
    {
        vel.y += speed;
    }

    SetVelocity(vel, speed);
}

void Bullet::OnRender(float alpha)
{
    Entities::OnRender(alpha, !m_active);
}
