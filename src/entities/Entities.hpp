#pragma once

#include <cmath>

#include "SDL3/SDL.h"

#include "../utils/Utils.hpp"

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

    void SetPosition(Vector2 position)
    {
        m_currentPosition.x = position.x;
        m_currentPosition.y = position.y;
    }
    
    void SetRect(SDL_FRect rect) {m_OriginalRect = rect; m_rect = rect; }

    void SetVelocity(Vector2 velocity, float speed)
    {
        // Normalize the velocity vector
        float length = sqrt(velocity.x * velocity.x + velocity.y * velocity.y);
        if (length > 0) {
            velocity.x /= length;
            velocity.y /= length;
        }

        // Scale the velocity to a desired speed
        velocity.x *= speed;
        velocity.y *= speed;

        m_velocity = velocity;
    }

    void SetStatus(bool status) { m_status = status; }

    void SetActive(bool active) { m_active = active; }

    SDL_FRect GetRect() { return m_rect; }

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

    float GetScaleFactor() { return m_scaleFactor; }

    bool GetStatus() { return m_status; }

    bool GetActive() { return m_active; }
    
    void OnUpdate(float dt);
    void OnRender(float alpha, bool entityVisible = false);

private:
    SDL_Window *m_window;
    SDL_Renderer *m_renderer;

    Utils* m_utils;

    float m_scaleFactor{};

    SDL_FRect m_rect{};
    SDL_FRect m_OriginalRect{};

    Vector2 m_velocity{};
    float FIXED_TIME_STEP = 1.0f / 60.0f;

    Vector2 m_previousPosition{};
    Vector2 m_currentPosition{};

    bool m_status{};

    void RenderSprite(const Vector2 &position);

    bool m_active{true};
};
