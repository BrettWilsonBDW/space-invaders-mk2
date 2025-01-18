#include "App.hpp"

#include <iostream>

App::App()
{
    if (!SDL_Init(SDL_INIT_VIDEO))
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't initialize SDL: %s", SDL_GetError());
        return;
    }

    if (!SDL_CreateWindowAndRenderer("Hello SDL", 1920 / 2, 1080 / 2, SDL_WINDOW_RESIZABLE, &m_window, &m_renderer))
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't create window and renderer: %s", SDL_GetError());
        return;
    }

    SDL_SetWindowRelativeMouseMode(m_window, true);

    SDL_SetRenderVSync(m_renderer, 1);

    m_game.Init(m_window, m_renderer);

    m_utils.Init(m_window, m_renderer);
    m_utils.SetBaseWindowSize(1920 / 2, 1080 / 2);
    m_utils.GetScaleFactor();

    m_lastTime = SDL_GetPerformanceCounter();

#ifdef __EMSCRIPTEN__
    emscripten_set_main_loop_arg(MainLoopHelper, this, 0, 1);
#else
    while (m_isRunning)
    {
        mainLoop();
    }
#endif
}

App::~App()
{
    SDL_DestroyRenderer(m_renderer);
    SDL_DestroyWindow(m_window);

    SDL_Quit();
}

void App::OnInput(SDL_Event *event)
{
    m_game.OnInput(event);
}

void App::OnUpdate(void)
{
    m_game.OnUpdate();
}

void App::OnRender(float alpha)
{
    m_game.OnRender(alpha);
}

void App::mainLoop(void)
{
    const float FRAME_TIME_ALPHA = 0.9f;  // Smoothing factor for frame time
    float avgFrameTime = FIXED_TIME_STEP; // Running average of frame time

    while (SDL_PollEvent(&m_event))
    {
        if (m_event.type == SDL_EVENT_QUIT)
        {
            m_isRunning = false;
        }
        if (m_event.key.key == SDLK_ESCAPE)
        {
            SDL_SetWindowRelativeMouseMode(m_window, false);
        }
        if (m_event.type == SDL_EVENT_MOUSE_BUTTON_DOWN && !SDL_GetWindowRelativeMouseMode(m_window))
        {
            SDL_SetWindowRelativeMouseMode(m_window, true);
        }
        OnInput(&m_event);
    }

    // Time calculation with smoothing
    Uint64 currentTime = SDL_GetPerformanceCounter();
    float elapsedTime = (currentTime - m_lastTime) / (float)SDL_GetPerformanceFrequency();
    m_lastTime = currentTime;

    // Smooth out frame time to reduce jarring from sudden spikes
    avgFrameTime = (FRAME_TIME_ALPHA * avgFrameTime) + ((1.0f - FRAME_TIME_ALPHA) * elapsedTime);

    // Accumulate time using smoothed frame time
    m_accumulatedTime += avgFrameTime;

    // Dynamic time step limiting based on current performance
    float maxAccumulatedTime = FIXED_TIME_STEP * 3.0f;
    if (avgFrameTime > FIXED_TIME_STEP * 2.0f)
    {
        // If we're running very slowly, allow more accumulated time
        maxAccumulatedTime = FIXED_TIME_STEP * 5.0f;
    }

    if (m_accumulatedTime > maxAccumulatedTime)
    {
        m_accumulatedTime = maxAccumulatedTime;
    }

    // Update game logic at fixed intervals with catch-up limiting
    int updateCount = 0;
    while (m_accumulatedTime >= FIXED_TIME_STEP && updateCount < MAX_UPDATES_PER_FRAME)
    {
        OnUpdate();
        m_accumulatedTime -= FIXED_TIME_STEP;
        updateCount++;
    }

    // Improved interpolation calculation
    float interpolation = m_accumulatedTime / FIXED_TIME_STEP;
    interpolation = std::min(std::max(interpolation, 0.0f), 1.0f);

    // If we're dropping frames, adjust interpolation to smooth movement
    if (updateCount >= MAX_UPDATES_PER_FRAME && m_accumulatedTime >= FIXED_TIME_STEP)
    {
        interpolation = 1.0f;
    }

    // Render
    SDL_SetRenderDrawColor(m_renderer, 0x00, 0x00, 0x00, 0x00);
    SDL_RenderClear(m_renderer);
    OnRender(interpolation);
    SDL_RenderPresent(m_renderer);

    const float TARGET_FRAME_TIME = 1.0f / TARGET_FPS;
    float remainingTime = TARGET_FRAME_TIME - elapsedTime;
    if (remainingTime > 0)
    {
        SDL_Delay((Uint32)(remainingTime * 1000));
    }
}

void App::MainLoopHelper(void *data)
{
    App *app = static_cast<App *>(data);
    app->mainLoop();
}
