#pragma once

#include <vector>

#include "SDL3/SDL.h"

#include "../Entities.hpp"

class Enemy : public Entities
{
public:
    Enemy(SDL_Window *window, SDL_Renderer *renderer);
    ~Enemy();

    void SetSpeed(float speed) { m_speed = speed; }

    void SetTrajectory(std::vector<Vector2> trajectory) { m_trajectory = trajectory; }
    void OnUpdate();
private:
    std::vector<Vector2> m_trajectory{};
    int m_trajectoryIndex{};

    void MoveThroughTrajectories();

    float m_speed{};

};
