#pragma once

#include "../entities/enemies/Enemy.hpp"
#include "../entities/bullet/Bullet.hpp"

#include "../entities/player/Player.hpp"
// #include "../levels/Levels.hpp"

#include <vector>

class Levels
{
public:
    Levels(SDL_Window *window, SDL_Renderer *renderer);
    ~Levels();

    Levels();

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

    void OnUpdate(float dt);
    void OnInput(SDL_Event *event);
    void OnRender(float alpha);

private:
    SDL_Window *m_window = nullptr;
    SDL_Renderer *m_renderer = nullptr;

    std::vector<Enemy *> m_enemies;
    // Player m_player = Player(m_window, m_renderer);
    Player *m_player = nullptr;

    // std::vector<Bullet> m_bullets;

    SDL_FRect* playerRectPtr = nullptr;

    int m_currentFiringEnemy{0};
    bool m_canFire{true};
};
