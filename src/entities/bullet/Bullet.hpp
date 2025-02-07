#pragma once

#include "SDL3/SDL.h"

#include "../Entities.hpp"

class Bullet : public Entities
{
public:
    Bullet(SDL_Window *window, SDL_Renderer *renderer);
    ~Bullet();

    void SwapDirection() { m_direction = !m_direction; }
    
    bool CheckCollisions(SDL_FRect a, SDL_FRect b);

    void OnUpdate(float dt);
    void OnRender(float alpha);
private:
    bool m_direction{};
};


