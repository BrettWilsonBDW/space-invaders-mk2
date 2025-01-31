#include "Entities.hpp"

#include <iostream>

Entities::Entities(SDL_Window *window, SDL_Renderer *renderer) : m_window(window), m_renderer(renderer), m_utils(&Utils::GetInstance()), m_OriginalRect(m_rect)
{
}

Entities::~Entities()
{
}

void Entities::OnUpdate(float dt)
{
    m_previousPosition = m_currentPosition;

    m_currentPosition.x += m_velocity.x * dt;
    m_currentPosition.y += m_velocity.y * dt;
}

void Entities::OnRender(float alpha, bool entityVisible)
{
    Vector2 renderPosition;

    renderPosition.x = m_previousPosition.x * (1.0f - alpha) + m_currentPosition.x * alpha;
    renderPosition.y = m_previousPosition.y * (1.0f - alpha) + m_currentPosition.y * alpha;

    m_scaleFactor = m_utils->GetScaleFactor();

    m_rect.x = static_cast<float>(renderPosition.x) * m_scaleFactor;
    m_rect.y = static_cast<float>(renderPosition.y) * m_scaleFactor;
    m_rect.w = m_OriginalRect.w * m_scaleFactor; // Width
    m_rect.h = m_OriginalRect.h * m_scaleFactor; // Height

    if (entityVisible)
    {
        return;
    }

    RenderSprite(renderPosition);
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