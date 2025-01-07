#pragma once

#include <iostream>
#include <vector>
#include <unordered_map>
#include <algorithm>

#include "SDL3/SDL.h"
#include "SDL3_image/SDL_image.h"

class ImageManager
{
public:
    ImageManager(SDL_Window *window = nullptr);
    ~ImageManager();
    
    void LoadAllImagesFromPath(std::string path);

    void SetSDLWindow(SDL_Window *window)
    {
        if (SDL_GetWindowID(window) == 0)
        {
            SDL_Log("The Sdl window has not been created yet. Use this method after the window has been created.\n");
        }

        if (window != nullptr)
        {
            this->m_window = window;
        }
    }

    SDL_Surface *loadImgSurfaceFromPath(std::string path);
    SDL_Texture *GetTextureFromPath(std::string path);
    SDL_Texture *GetTexture(const std::string &fileName);
private:
    std::vector<std::string> FetchAssetPaths(std::string path);

    SDL_Window *m_window;

    std::unordered_map<std::string, void *> m_assets;
    std::vector<SDL_Surface *> m_unloadSurfaces;
    std::vector<SDL_Texture *> m_unloadTextures;
#ifdef DEBUG
    int optimizeMemUseCtr{};
#endif
};