#include "LevelLoader.hpp"

#include <iostream>

LevelLoader::LevelLoader() {}

LevelLoader::LevelLoader(SDL_Window *window, SDL_Renderer *renderer) : m_window(window), m_renderer(renderer)
{
    m_player = new Player(m_window, m_renderer);

    LoadLevel(0);
}

LevelLoader::~LevelLoader()
{
    for (auto &enemy : m_enemies)
    {
        delete enemy;
    }

    delete m_player;
}

void LevelLoader::LoadLevel(int level)
{
    m_enemies.clear();

    const float enemyWidth = m_levelData.GetLevelData(level).width;
    const int enemyGap = m_levelData.GetLevelData(level).gap;
    const int numEnemies = m_levelData.GetLevelData(level).enemyCount;

    // Calculate total width of all entities+
    float totalWidth = (enemyWidth + enemyGap) * numEnemies - enemyGap;

    // Calculate offset to center entities
    float offsetX = (GetWindowWidth() - totalWidth) / 2;

    // load up enemies and set their data
    for (int i = 0; i < numEnemies; i++)
    {
        const float displacement = (offsetX + (float)(i * (enemyWidth + enemyGap)));

        m_enemies.push_back(new Enemy(m_window, m_renderer));
        m_enemies[i]->SetId(i);
        m_enemies[i]->SetRect({displacement, 0, enemyWidth, enemyWidth});
        m_enemies[i]->SetPosition({displacement, 0});
        m_enemies[i]->SetSpeed(m_levelData.GetLevelData(level).enemySpeed);

        m_enemies[i]->SetTrajectory(m_levelData.GetLevelData(0).trajectory);

        m_enemies[i]->SetPlayer(m_player);
    }

    // for collisions
    for (auto &enemy : m_enemies)
    {
        m_player->SetEnemies(enemy);
    }
}

void LevelLoader::UpdateEnemies(float dt)
{
    // Update all enemies
    for (auto &enemy : m_enemies)
    {
        enemy->OnUpdate(dt);
    }

    // enemy bullet firing
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

    // check if the current enemy can fire
    if (!m_enemies[m_currentFiringEnemy]->GetCanFire())
    {
        m_canFire = true;
    }

    // get the number of active enemies
    int activeEnemies = m_enemies.size();
    for (auto &enemy : m_enemies)
    {
        if (!enemy->GetActive())
        {
            activeEnemies--;
        }
    }

    // if the there is only one active enemy then follow the player and speed up
    if (activeEnemies == 1)
    {
        m_enemies[m_currentFiringEnemy]->SetTrajectory({{((float)m_player->GetRect().x + ((m_player->GetRect().w / 2) / 2)) / m_player->GetScaleFactor(), 0}});
        m_enemies[m_currentFiringEnemy]->SetSpeed(400);
    }

    if (activeEnemies == 0)
    {
        m_currentFiringEnemy = 0;

        levelNum++;
    }
}

void LevelLoader::OnUpdate(float dt)
{
    UpdateEnemies(dt);

    // check if the level has changed
    if (levelNum != currentLevel)
    {
        currentLevel = levelNum;

        if (levelNum > m_levelData.GetAllLevels().size() - 1)
        {
            levelNum = 0;
            return;
        }

        LoadLevel(levelNum);
    }

    m_player->OnUpdate(dt);
}

void LevelLoader::OnInput(SDL_Event *event)
{
    m_player->OnInput(event);
}

void LevelLoader::OnRender(float alpha)
{
    for (auto &enemy : m_enemies)
    {
        enemy->OnRender(alpha);
    }

    m_player->OnRender(alpha);
}
