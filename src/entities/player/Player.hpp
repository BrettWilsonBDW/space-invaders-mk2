#pragma once

#include <vector>

#include "SDL3/SDL.h"

#include "../Entities.hpp"

#include "../bullet/Bullet.hpp"
#include "../enemies/Enemy.hpp"

#include <iostream>

#include <memory>

class Player : public Entities
{
public:
    Player(SDL_Window *window, SDL_Renderer *renderer);
    ~Player();

    void SetEnemies(Enemy* enemies) { m_enemies.push_back(enemies);}

    void OnUpdate(float dt);
    void OnInput(SDL_Event *event);
    void OnRender(float alpha);
private:
    bool keyW{};
    bool keyA{};
    bool keyS{};
    bool keyD{};
    bool keySpace{};

    bool leftLock{};
    bool rightLock{};

    std::vector<Enemy*> m_enemies;

    std::vector<Bullet> m_bullets;

    int m_bulletCount{};
    int m_fireCooldown{};
    int m_currentHealth{};
};
