#include "Player.hpp"

#include <iostream>

Player::Player(SDL_Window *window, SDL_Renderer *renderer) : Entities(window, renderer)
{
    SetTexture("playership.png");

    SetRect({100, 0, 100, 100});

    m_currentHealth = 3;

    SetPosition({(static_cast<float>(GetWindowWidth()) / 2) - 50, static_cast<float>(GetWindowHeight()) - 100});

    for (int i = 0; i < 3; i++)
    {
        m_bullets.push_back(Bullet(window, renderer));
    }
}

Player::~Player()
{
    for (auto &enemy : m_enemies)
    {
        if (enemy != nullptr)
        {
            delete enemy;
        }
    }
}

void Player::OnUpdate(float dt)
{
    Entities::OnUpdate(dt);

    Vector2 vel = {0, 0};

    float speed{45000};

    speed = speed * (1.0f / 60.0f);

    // left
    if (keyA && !leftLock)
    {
        rightLock = false;
        vel.x -= speed;
    }
    // right
    if (keyD && !rightLock)
    {
        leftLock = false;
        vel.x += speed;
    }

    float scaleFactor = GetScaleFactor();

    // left
    if (GetRect().x * scaleFactor < 0)
    {
        leftLock = true;
        SetPosition({0, GetRect().y / scaleFactor});
    }

    // right
    if (((GetRect().x) + GetRect().w) > GetWindowWidth())
    {
        rightLock = true;
        SetPosition({((float)GetWindowWidth() - (GetRect().w)) / scaleFactor, GetRect().y / scaleFactor});
    }

    SetVelocity(vel, speed);

    for (auto &bullet : m_bullets)
    {
        bullet.OnUpdate(dt);
    }

    if (keySpace && m_fireCooldown <= 0)
    {
        m_fireCooldown = 20;

        if (!m_bullets[m_bulletCount].GetActive())
        {
            m_bullets[m_bulletCount].SetActive(true);
            m_bullets[m_bulletCount].SetPosition({(GetRect().x + GetRect().w / 2) / scaleFactor, GetRect().y / scaleFactor});

            m_bulletCount++;

            if (m_bulletCount >= m_bullets.size())
            {
                m_bulletCount = 0;
            }
        }

        keySpace = false;
    }

    m_fireCooldown -= 1;

    for (auto &bullet : m_bullets)
    {
        if (!bullet.GetActive())
        {
            bullet.SetPosition({(GetRect().x + GetRect().w / 2) / scaleFactor, GetRect().y / scaleFactor});
        }

        for (auto &enemy : m_enemies)
        {
            if (bullet.CheckCollisions(bullet.GetRect(), enemy->GetRect()))
            {
                bullet.SetActive(false);
                enemy->SetActive(false);
            }

            if (bullet.GetRect().y < 0)
            {
                bullet.SetActive(false);
            }
        }
    }

    if (GetStatus())
    {
        m_currentHealth--;
        SetStatus(false);
    }

    if (m_currentHealth == 0)
    {
        SetActive(false);
    }
}

void Player::OnInput(SDL_Event *event)
{
    if (event->type == SDL_EVENT_KEY_DOWN || event->type == SDL_EVENT_KEY_UP)
    {
        bool isPressed = (event->type == SDL_EVENT_KEY_DOWN);

        switch (event->key.key)
        {
        case SDLK_W:
            keyW = isPressed;
            break;
        case SDLK_A:
            keyA = isPressed;
            break;
        case SDLK_S:
            keyS = isPressed;
            break;
        case SDLK_D:
            keyD = isPressed;
            break;
        case SDLK_SPACE:
            keySpace = isPressed;
            break;
        default:
            break;
        }
    }
}

void Player::OnRender(float alpha)
{
    Entities::OnRender(alpha, !GetActive());

    for (auto &bullet : m_bullets)
    {
        bullet.OnRender(alpha);
    }
}