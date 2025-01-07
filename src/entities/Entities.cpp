#include "Entities.hpp"

Entities::Entities(SDL_Window *window, SDL_Renderer *renderer) : m_window(window), m_renderer(renderer)
{

}

Entities::~Entities()
{
}

void Entities::OnUpdate()
{
    m_previousPosition = m_currentPosition;

    m_currentPosition.x += m_velocity.x * FIXED_TIME_STEP;
    m_currentPosition.y += m_velocity.y * FIXED_TIME_STEP;
}

void Entities::OnRender(float alpha)
{
    Vector2 interpolatedPosition;
    interpolatedPosition.x = m_previousPosition.x * (1.0f - alpha) +
                             m_currentPosition.x * alpha;
    interpolatedPosition.y = m_previousPosition.y * (1.0f - alpha) +
                             m_currentPosition.y * alpha;

    RenderSprite(interpolatedPosition);
}

void Entities::RenderSprite(const Vector2 &position)
{
    SDL_FRect destRect;

    destRect.x = static_cast<float>(position.x);
    destRect.y = static_cast<float>(position.y);
    destRect.w = m_rect.w; // Width
    destRect.h = m_rect.h; // Height

    SDL_SetRenderDrawColor(m_renderer, 255, 0, 0, 255);
    SDL_RenderRect(m_renderer, &destRect);

    m_rect = destRect;
}