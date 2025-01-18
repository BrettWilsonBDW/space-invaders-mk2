#pragma once

#include <SDL3/SDL.h>

class Utils
{
public:
    static Utils &GetInstance()
    {
        static Utils instance;
        return instance;
    }

    void Init(SDL_Window *window, SDL_Renderer *renderer)
    {
        if (m_initialized)
        {
            return;
        }

        m_window = window;
        m_renderer = renderer;
        m_initialized = true;
    }

    void SetBaseWindowSize(int width, int height) { m_baseWindowWidth = width; m_baseWindowHeight = height; }

    float GetScaleFactor()
    {
        CalculateScaleFactor(m_baseWindowWidth, m_baseWindowHeight);
        return m_scaleFactor;
    }

private:
    Utils() = default;

    Utils(const Utils &) = delete;
    Utils &operator=(const Utils &) = delete;

    bool m_initialized{false};

    SDL_Window *m_window;
    SDL_Renderer *m_renderer;

    int m_baseWindowWidth{};
    int m_baseWindowHeight{};

    float m_scaleFactor{};

    void CalculateScaleFactor(int baseWindowWidth, int baseWindowHeight);
};
