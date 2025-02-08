#include "App.hpp"

#include <iostream>
#include <algorithm>

#ifdef __EMSCRIPTEN__
EM_JS(int, getBrowserWidth, (), {
    return window.innerWidth;
});

EM_JS(int, getBrowserHeight, (), {
    return window.innerHeight;
});

extern "C"
{
    void rotateCanvasIfNeeded()
    {
        int width = getBrowserWidth();
        int height = getBrowserHeight();

        // Get the canvas element and adjust it based on orientation
        EM_ASM({
            var canvas = document.getElementById('canvas');
            console.log('Checking rotation');
            if ($0 < $1) {
                canvas.style.transform = 'rotate(90deg)';
            } else {
                canvas.style.transform = 'rotate(0deg)';
            } }, width, height);
    }
}
#endif

App::App()
{
    if (!SDL_Init(SDL_INIT_VIDEO))
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't initialize SDL: %s", SDL_GetError());
        return;
    }

    if (!SDL_CreateWindowAndRenderer("Space Invaders Clone MK2", 1920 / 2, 1080 / 2, 0, &m_window, &m_renderer))
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't create window and renderer: %s", SDL_GetError());
        return;
    }

    SDL_SetWindowRelativeMouseMode(m_window, true);
    
    m_utils.Init(m_window, m_renderer);
    m_utils.SetBaseWindowSize(1920 / 2, 1080 / 2);
    m_utils.GetScaleFactor();

    m_game.Init(m_window, m_renderer);

    m_lastTime = SDL_GetPerformanceCounter();
    m_frequency = (float)SDL_GetPerformanceFrequency();

    // SDL_SetWindowFullscreen(m_window, true);

#ifdef __EMSCRIPTEN__
    SDL_SetRenderVSync(m_renderer, 1);
    emscripten_set_main_loop_arg(MainLoopHelper, this, 0, 1);
#else
    while (m_isRunning)
    {
        Uint64 frameStartTime = SDL_GetPerformanceCounter();
        const Uint64 frameTime = m_frequency / (m_frameRate); // Target time for 60 FPS

        mainLoop();

        Uint64 frameEndTime = SDL_GetPerformanceCounter();
        Uint64 frameDuration = frameEndTime - frameStartTime;

        if (frameDuration < frameTime)
        {
            Uint64 waitTime = frameTime - frameDuration;
            SDL_DelayNS(waitTime);
        }
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

void App::OnUpdate(float elapsedTime)
{
    m_game.OnUpdate(elapsedTime);
}

void App::OnRender(float alpha)
{
    m_game.OnRender(alpha);
}

void App::mainLoop(void)
{
#ifdef __EMSCRIPTEN__
    rotateCanvasIfNeeded();
    // SDL_SetWindowSize(m_window, getBrowserWidth(), getBrowserHeight());

#endif

    // Fixed timestep based on https://gafferongames.com/post/fix_your_timestep/
    Uint64 currentTime = SDL_GetPerformanceCounter();
    float elapsedTime = (currentTime - m_lastTime) / static_cast<float>(m_frequency);
    m_lastTime = currentTime;

    while (SDL_PollEvent(&m_event))
    {
        if (m_event.type == SDL_EVENT_QUIT)
        {
            m_isRunning = false;
        }
        else if (m_event.key.key == SDLK_ESCAPE)
        {
            SDL_SetWindowRelativeMouseMode(m_window, false);
            m_paused = 0;
        }
        else if (m_event.type == SDL_EVENT_MOUSE_BUTTON_DOWN && !SDL_GetWindowRelativeMouseMode(m_window))
        {
            SDL_SetWindowRelativeMouseMode(m_window, true);
            m_paused = 1;
        }

        OnInput(&m_event);
    }

    // Accumulate time using smoothed frame time
    m_accumulatedTime = std::min(m_accumulatedTime + elapsedTime, FIXED_TIME_STEP * 5.0f); // Cap accumulated time

    // Update game logic at fixed intervals with catch-up limiting
    int updateCount = 0;
    while (m_accumulatedTime >= FIXED_TIME_STEP && updateCount <= 5)
    {
        OnUpdate(FIXED_TIME_STEP * m_paused);
        m_accumulatedTime -= FIXED_TIME_STEP;
        updateCount++;

        if (updateCount > 5)
        {
            break;
        }
    }

    float alpha = std::clamp(m_accumulatedTime / FIXED_TIME_STEP, 0.0f, 1.0f);

    // Render
    SDL_SetRenderDrawColor(m_renderer, 0x00, 0x00, 0x00, 0x00);
    SDL_RenderClear(m_renderer);
    OnRender(alpha);
    SDL_RenderPresent(m_renderer);
}

void App::MainLoopHelper(void *data)
{
    App *app = static_cast<App *>(data);
    app->mainLoop();
}
