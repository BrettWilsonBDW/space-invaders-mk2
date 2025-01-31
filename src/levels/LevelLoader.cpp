#include "LevelLoader.hpp"

#include <iostream>

Levels::Levels() {}

Levels::Levels(SDL_Window *window, SDL_Renderer *renderer) : m_window(window), m_renderer(renderer)
{
    m_player = new Player(m_window, m_renderer);

    playerRectPtr = new SDL_FRect(m_player->GetRect());

    const int enemyWidth = 50;
    const int enemyGap = 20;
    const int numEnemies = 5;

    // Calculate total width of all entities+
    float totalWidth = (enemyWidth + enemyGap) * numEnemies - enemyGap;

    // Calculate offset to center entities
    float offsetX = (GetWindowWidth() - totalWidth) / 2;

    for (int i = 0; i < numEnemies; i++)
    {
        const float displacement = (offsetX + (float)(i * (enemyWidth + enemyGap)));

        m_enemies.push_back(new Enemy(m_window, m_renderer));
        m_enemies[i]->SetId(i);
        m_enemies[i]->SetRect({displacement, 0, enemyWidth, enemyWidth});
        m_enemies[i]->SetPosition({displacement, 0});
        m_enemies[i]->SetSpeed(300);

        m_enemies[i]->SetTrajectory({{100, 0}, {800, 0}});

        m_enemies[i]->SetPlayer(m_player);
    }

    for (auto &enemy : m_enemies)
    {
        m_player->SetEnemies(enemy);
    }
}

Levels::~Levels()
{
    for (auto &enemy : m_enemies)
    {
        delete enemy;
    }

    delete m_player;

    delete playerRectPtr;
}

void Levels::OnUpdate(float dt)
{
    // Update all enemies
    for (auto &enemy : m_enemies)
    {
        enemy->OnUpdate(dt);
    }

    if (m_canFire)
    {
        int tries = m_enemies.size();
        bool foundActiveEnemy = false;

        while (tries > 0)
        {
            m_currentFiringEnemy = SDL_rand(m_enemies.size()) % m_enemies.size();

            if (m_enemies[m_currentFiringEnemy]->GetActive())
            {
                foundActiveEnemy = true;
                break;
            }

            tries--;
        }

        if (foundActiveEnemy)
        {
            m_enemies[m_currentFiringEnemy]->FireBullet();
            m_canFire = false;
        }
    }

    if (!m_enemies[m_currentFiringEnemy]->GetCanFire())
    {
        m_canFire = true;
    }

    m_enemies[0]->SetTrajectory({{(float)m_player->GetRect().x, 0}});

    m_player->OnUpdate(dt);
}

void Levels::OnInput(SDL_Event *event)
{
    m_player->OnInput(event);
}

void Levels::OnRender(float alpha)
{
    for (auto &enemy : m_enemies)
    {
        enemy->OnRender(alpha);
    }

    m_player->OnRender(alpha);
}