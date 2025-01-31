#pragma once

#include <vector>

#include "SDL3/SDL.h"

#include "../Entities.hpp"

#include "../bullet/Bullet.hpp"

class Enemy : public Entities
{
public:
    Enemy(SDL_Window *window, SDL_Renderer *renderer);
    ~Enemy();

    void SetId(int id) { m_id = id; }
    void SetSpeed(float speed) { m_speed = speed; }
    void SetActive(bool active) { m_active = active; }
    void SetPlayer(Entities *player) { m_player = player; }
    void SetTrajectory(std::vector<Vector2> trajectory) { m_trajectory = trajectory; }
    bool GetActive() { return m_active; }
    bool GetCanFire() { return firedOnce; }
    
    void FireBullet() { if (!firedOnce) {m_fired = true; firedOnce = true;} }

    void OnUpdate(float dt);
    void OnRender(float alpha);
private:
    std::vector<Vector2> m_trajectory{};
    int m_trajectoryIndex{};
    float m_speed{};
    bool m_active{true};
    std::vector<Bullet> m_bullets;
    bool m_fired{};
    bool firedOnce{};

    int m_id{};

    SDL_FRect *m_playerRect{};

    Entities *m_player;

    void ShootBullets();
    void MoveThroughTrajectories();
};
