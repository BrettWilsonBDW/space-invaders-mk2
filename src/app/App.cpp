#include "App.hpp"

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

    const int linked = SDL_GetVersion();
    std::cout << "SDL version: " << SDL_VERSIONNUM_MAJOR(linked) << "." << SDL_VERSIONNUM_MINOR(linked) << "." << SDL_VERSIONNUM_MICRO(linked) << std::endl;

    SDL_SetWindowRelativeMouseMode(m_window, true);

    SDL_SetRenderVSync(m_renderer, 1);

    m_player = Player(m_window, m_renderer);

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
    m_player.OnInput(event);
}

void App::OnUpdate(void)
{
    m_player.OnUpdate();
}

void App::OnRender(float alpha)
{
    m_player.OnRender(alpha);
}

void App::mainLoop(void)
{
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

    // Time calculation
    Uint64 currentTime = SDL_GetPerformanceCounter();
    float elapsedTime = (currentTime - m_lastTime) / (float)SDL_GetPerformanceFrequency();
    m_lastTime = currentTime;

    // Accumulate time
    m_accumulatedTime += elapsedTime;

    // Prevent accumulated time from growing too large
    if (m_accumulatedTime > FIXED_TIME_STEP * 5.0f)
    {
        m_accumulatedTime = FIXED_TIME_STEP * 5.0f;
    }

    // Update game logic at fixed intervals
    int updateCount = 0;
    while (m_accumulatedTime >= FIXED_TIME_STEP && updateCount < MAX_UPDATES_PER_FRAME)
    {
        OnUpdate();
        m_accumulatedTime -= FIXED_TIME_STEP;
        updateCount++;
    }

    float interpolation = m_accumulatedTime / FIXED_TIME_STEP;
    if (interpolation > 1.0f)
    {
        interpolation = 1.0f;
    }

    // Render
    SDL_SetRenderDrawColor(m_renderer, 0x00, 0x00, 0x00, 0x00);
    SDL_RenderClear(m_renderer);
    OnRender(interpolation);
    SDL_RenderPresent(m_renderer);
}

void App::MainLoopHelper(void *data)
{
    App *app = static_cast<App *>(data);
    app->mainLoop();
}
