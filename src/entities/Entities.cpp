#include "Entities.hpp"

Entities::Entities(SDL_Window *window, SDL_Renderer *renderer) : m_window(window), m_renderer(renderer), m_utils(&Utils::GetInstance()), m_OriginalRect(m_rect)
{

}

Entities::~Entities()
{
}

void Entities::CheckCollisions()
{
}

void Entities::OnUpdate()
{
    m_previousPosition = m_currentPosition;

    m_currentPosition.x += m_velocity.x * FIXED_TIME_STEP;
    m_currentPosition.y += m_velocity.y * FIXED_TIME_STEP;
}

void Entities::OnRender(float alpha, bool entityVisible)
{
    Vector2 interpolatedPosition;
    interpolatedPosition.x = m_previousPosition.x * (1.0f - alpha) +
                             m_currentPosition.x * alpha;
    interpolatedPosition.y = m_previousPosition.y * (1.0f - alpha) +
                             m_currentPosition.y * alpha;


    m_scaleFactor = m_utils->GetScaleFactor();

    m_rect.x = static_cast<float>(interpolatedPosition.x) * m_scaleFactor;
    m_rect.y = static_cast<float>(interpolatedPosition.y) * m_scaleFactor;
    m_rect.w = m_OriginalRect.w * m_scaleFactor; // Width
    m_rect.h = m_OriginalRect.h * m_scaleFactor; // Height

    if (entityVisible)
    {
        return;
    }

    RenderSprite(interpolatedPosition);
}

void Entities::RenderSprite(const Vector2 &position)
{
    SDL_FRect destRect;

    destRect.x = static_cast<float>(position.x) * m_scaleFactor;
    destRect.y = static_cast<float>(position.y) * m_scaleFactor;
    destRect.w = m_OriginalRect.w * m_scaleFactor; // Width
    destRect.h = m_OriginalRect.h * m_scaleFactor; // Height

    SDL_SetRenderDrawColor(m_renderer, 255, 0, 0, 255);
    SDL_RenderRect(m_renderer, &destRect);
}