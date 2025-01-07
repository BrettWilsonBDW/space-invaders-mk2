#include "Player.hpp"

#include <iostream>

Player::Player(SDL_Window *window, SDL_Renderer *renderer) : Entities(window, renderer)
{
    int width, height;
    SDL_GetWindowSize(window, &width, &height);

    std::cout << "Window size: " << width << "x" << height << std::endl;
    SetRect({100, 0, 100, 100});

    SetPosition({(static_cast<float>(width) / 2) - 50, static_cast<float>(height) - 100});
}

Player::~Player()
{
}

void Player::OnUpdate()
{
    Entities::OnUpdate();

    Vector2 vel = {0, 0};

    float speed{35000};

    speed =  speed * (1.0f / 60.0f);

    if (keyW)
    {
        vel.y -= speed; // up
    }
    if (keyS)
    {
        vel.y += speed; // down
    }
    if (keyA)
    {
        vel.x -= speed; // left
    }
    if (keyD)
    {
        vel.x += speed; // right
    }

    SetVelocity(vel);
}

void Player::OnInput(SDL_Event *event)
{
    if (event->type == SDL_EVENT_KEY_DOWN || event->type == SDL_EVENT_KEY_UP)
    {
        bool isPressed = (event->type == SDL_EVENT_KEY_DOWN);
        
        switch (event->key.key)
        {
            case SDLK_W:
                keyW = isPressed;
                break;
            case SDLK_A:
                keyA = isPressed;
                break;
            case SDLK_S:
                keyS = isPressed;
                break;
            case SDLK_D:
                keyD = isPressed;
                break;
        }
    }
}