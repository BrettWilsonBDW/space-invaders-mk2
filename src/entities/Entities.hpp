#pragma once

#include <SDL3/SDL.h>

struct Vector2
{
    float x{};
    float y{};
};

class Entities
{
public:
    Entities(SDL_Window *window, SDL_Renderer *renderer);
    ~Entities();

    void SetPosition(Vector2 position) { m_currentPosition.x = position.x; m_currentPosition.y = position.y; }
    void SetRect(SDL_FRect rect) { m_rect = rect;}
    void SetVelocity(Vector2 velocity) { m_velocity = velocity; }

    SDL_FRect GetRect() { return m_rect; }

    void OnUpdate();
    void OnRender(float alpha);

private:
    SDL_Window *m_window;
    SDL_Renderer *m_renderer;

    SDL_FRect m_rect{};

    Vector2 m_velocity{};
    float FIXED_TIME_STEP = 1.0f / 60.0f;

    Vector2 m_previousPosition{};
    Vector2 m_currentPosition{};

    void RenderSprite(const Vector2& position);
};
