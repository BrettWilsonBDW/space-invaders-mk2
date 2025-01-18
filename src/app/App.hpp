#pragma once


#include <vector>

#include "SDL3/SDL.h"

// #include "../entities/player/Player.hpp"
// #include "../levels/Levels.hpp"
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
    Uint64 m_lastTime{};
    float m_accumulatedTime = 0.0f;
    const float FIXED_TIME_STEP = 1.0f / 60.0f;
    const float TARGET_FPS = 60.0f;
    const int MAX_UPDATES_PER_FRAME = 5;

    SDL_Window *m_window = nullptr;
    SDL_Renderer *m_renderer = nullptr;
    SDL_Event m_event;

    Game m_game = Game(m_window, m_renderer);
    // Player m_player = Player(m_window, m_renderer);
    // Levels m_levels = Levels(m_window, m_renderer);
    // Levels m_levels;
    // Utils m_utils = Utils(m_window, m_renderer);
    Utils& m_utils = Utils::GetInstance();


    static void MainLoopHelper(void *data);
    void mainLoop(void);

    void OnInput(SDL_Event *event);
    void OnUpdate();
    void OnRender(float alpha);
};
