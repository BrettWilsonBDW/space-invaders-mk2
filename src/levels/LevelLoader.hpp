#pragma once

#include "../entities/enemies/Enemy.hpp"
#include "../entities/bullet/Bullet.hpp"

#include "../entities/player/Player.hpp"

#include "levelData/LevelData.hpp"

#include <vector>

class LevelLoader
{
public:
    LevelLoader(SDL_Window *window, SDL_Renderer *renderer);
    ~LevelLoader();

    LevelLoader();

    void OnUpdate(float dt);
    void OnInput(SDL_Event *event);
    void OnRender(float alpha);

private:
    SDL_Window *m_window = nullptr;
    SDL_Renderer *m_renderer = nullptr;

    Player *m_player = nullptr;
    LevelData m_levelData{};

    int m_currentFiringEnemy{0};
    bool m_canFire{true};

    std::vector<Enemy *> m_enemies;

    int levelNum{0};
    int currentLevel{0};

    void UpdateEnemies(float dt);

    void LoadLevel(int level);

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
