#pragma once

#include <vector>

#include "SDL3/SDL.h"

#include "../utils/Utils.hpp"
#include "game/Game.hpp"

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

class App
{
public:
    App();
    ~App();
private:
    bool m_isRunning{1};
    float m_frameRate{60};
    const float FIXED_TIME_STEP = 1.0f / m_frameRate;
    Uint64 m_lastTime{};
    float m_accumulatedTime = 0.0f;
    float m_frequency{};

    SDL_Window *m_window = nullptr;
    SDL_Renderer *m_renderer = nullptr;
    SDL_Event m_event;

    Game m_game = Game(m_window, m_renderer);
    Utils& m_utils = Utils::GetInstance();

    int m_paused{1};

    static void MainLoopHelper(void *data);
    void mainLoop(void);

    void OnInput(SDL_Event *event);
    void OnUpdate(float dt);
    void OnRender(float alpha);
};
