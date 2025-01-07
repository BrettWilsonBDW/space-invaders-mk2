#pragma once

#include <SDL3/SDL.h>

#include "../Entities.hpp"

class Player : public Entities
{
public:
    Player(SDL_Window *window, SDL_Renderer *renderer);
    Player();
    ~Player();

    void OnUpdate();
    void OnInput(SDL_Event *event);

private:
    bool keyW = false;
    bool keyA = false;
    bool keyS = false;
    bool keyD = false;
};
