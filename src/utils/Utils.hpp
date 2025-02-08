#pragma once

#include <SDL3/SDL.h>

#include "../imageManager/ImageManager.hpp"

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

        imageManager.SetSDLWindow(m_window);

        imageManager.LoadAllImagesFromPath("./assets");
    }

    void SetBaseWindowSize(int width, int height)
    {
        m_baseWindowWidth = width;
        m_baseWindowHeight = height;
    }

    float GetScaleFactor()
    {
        CalculateScaleFactor(m_baseWindowWidth, m_baseWindowHeight);
        return m_scaleFactor;
    }

    SDL_Texture *GetTexture(std::string name) { return imageManager.GetTexture(name); }

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

private:
    Utils() = default;

    Utils(const Utils &) = delete;
    Utils &operator=(const Utils &) = delete;

    bool m_initialized{false};

    SDL_Window *m_window;
    SDL_Renderer *m_renderer;

    ImageManager imageManager;

    int m_baseWindowWidth{};
    int m_baseWindowHeight{};

    float m_scaleFactor{};

    void CalculateScaleFactor(int baseWindowWidth, int baseWindowHeight);
};
