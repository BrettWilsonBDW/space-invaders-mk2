#include "ImageManager.hpp"

ImageManager::ImageManager(SDL_Window *window) : m_window(window)
{
    if (SDL_GetWindowID(window) == 0 && window != nullptr)
    {
        SDL_Log("The Sdl window has not been created yet. Set it with SetSDLWindow() or pass it to the constructor after the window has been created.");
    }

    // int imgFlags = IMG_INIT_PNG | IMG_INIT_JPG | IMG_INIT_WEBP;
    // if (!(IMG_Init(imgFlags) & imgFlags))
    // {
    //     SDL_Log("SDL_image could not initialize! SDL_image Error: %s\n", SDL_GetError());
    // }
}

ImageManager::~ImageManager()
{
    // unload surfaces
    for (auto &surface : m_unloadSurfaces)
    {
        SDL_DestroySurface(surface);
    }

    // unload textures
    for (auto &texture : m_unloadTextures)
    {
        SDL_DestroyTexture(texture);
    }

    // unload textures from map
    for (const auto &pair : m_assets)
    {
        SDL_DestroyTexture(static_cast<SDL_Texture *>(pair.second));
    }

    // IMG_Quit();
}

/**
 * @brief Loads an surface from a given file path and optimizes it for the display.
 *
 * This function loads an surface from a given file path and optimizes it for the display
 * by converting it to the screen's format. If the surface could not be loaded or optimized,
 * a nullptr is returned and an error message is printed to the SDL log.
 *
 * @param path The file path to the surface to load.
 * @return A SDL_Surface* representing the loaded surface, or nullptr on error.
 */
SDL_Surface *ImageManager::loadImgSurfaceFromPath(std::string path)
{
    SDL_Surface *optimizedSurface = NULL;

    SDL_Surface *loadedSurface = IMG_Load(path.c_str());
    if (loadedSurface == NULL)
    {
        SDL_Log("Unable to load image %s! SDL_image Error: %s\n", path.c_str(), SDL_GetError());
    }
    else
    {
        // Convert surface to screen format
        optimizedSurface = SDL_ConvertSurface(loadedSurface, SDL_GetWindowSurface(m_window)->format);
        if (optimizedSurface == NULL)
        {
            SDL_Log("Unable to optimize image %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
        }

        SDL_DestroySurface(loadedSurface);
    }

    m_unloadSurfaces.push_back(optimizedSurface);

#ifdef DEBUG
    if (m_unloadSurfaces.size() == 200)
    {
        SDL_Log("Warning: Memory leak detected: texture of %s was loaded %llu times, resulting in unnecessary memory allocation.", path.c_str(), m_unloadSurfaces.size());
    }
#endif

    return optimizedSurface;
}

/**
 * @brief Returns a SDL_Texture* from a given image file path.
 *
 * This function loads a surface from an image file at the given path and
 * creates a SDL_Texture* from it. If any errors occur, it returns a nullptr.
 * The SDL_Texture* is destroyed when the ImageManager is destroyed.
 *
 * @param path The path to the image file.
 * @return A SDL_Texture* or nullptr if an error occurs.
 */
SDL_Texture *ImageManager::GetTextureFromPath(std::string path)
{
    if (path.empty())
    {
        SDL_Log("Please provide a file name\n");
        return nullptr;
    }

    SDL_Surface *surface = loadImgSurfaceFromPath(path.c_str());
    if (surface == NULL)
    {
        return nullptr;
    }

    SDL_Texture *texture = SDL_CreateTextureFromSurface(SDL_GetRenderer(m_window), surface);
    if (texture == NULL)
    {
        SDL_Log("Unable to create texture from image %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
        return nullptr;
    }

    if (surface != NULL)
    {
        SDL_DestroySurface(surface);
    }

    m_unloadTextures.push_back(texture);

#ifdef DEBUG
    if (m_unloadTextures.size() == 200)
    {
        SDL_Log("Warning: Memory leak detected: texture of %s was loaded %llu times, resulting in unnecessary memory allocation.", path.c_str(), m_unloadTextures.size());
    }
#endif

    return texture;
}

/**
 * @brief Fetches all asset paths in a given directory and its subdirectories.
 * @param path The path to the directory from which to fetch the asset paths.
 * @return A vector of strings, each containing a path to an asset file.
 */
std::vector<std::string> ImageManager::FetchAssetPaths(std::string path)
{
    std::vector<std::string> allfiles;
    std::string patterns[] = {"*.png", "*.jpg", "*.webp"};

    for (size_t i = 0; i < sizeof(patterns) / sizeof(patterns[0]); i++)
    {
        int count{};
        char **matches = SDL_GlobDirectory(path.c_str(), patterns[i].c_str(), 0, &count);

        for (int j = 0; j < count; j++)
        {
            allfiles.push_back(matches[j]);
        }
    }

    std::vector<std::string> relativePaths;
    for (size_t i = 0; i < allfiles.size(); i++)
    {
        std::string relativePath = "./" + path + "/" + allfiles[i];
        // std::cout << relativePath << std::endl;

        relativePaths.push_back(relativePath);
    }

    return relativePaths;
}

/**
 * @brief Loads all images from a given path and stores them in the ImageManager map.
 *
 * This function loads all images from the given path and stores them in the ImageManager map.
 * The keys are the filenames of the images without the path or extension, and the values are the
 * corresponding SDL_Texture pointers. If an image can't be loaded, it's skipped.
 *
 * @param path The path to the directory from which to load the images.
 */
void ImageManager::LoadAllImagesFromPath(std::string path)
{
    for (const auto &file : FetchAssetPaths(path))
    {
        SDL_Surface *surface = loadImgSurfaceFromPath(file.c_str());
        if (surface == NULL)
        {
            continue;
        }

        SDL_Texture *texture = SDL_CreateTextureFromSurface(SDL_GetRenderer(m_window), surface);
        if (texture == NULL)
        {
            SDL_Log("Unable to create texture from image %s! SDL Error: %s\n", file.c_str(), SDL_GetError());
            continue;
        }

        if (surface != NULL)
        {
            SDL_DestroySurface(surface);
        }

        std::string fileName = file.substr(file.find_last_of('/') + 1);
        fileName = fileName.substr(0, fileName.find_last_of('.'));

        m_assets.emplace(fileName, texture);
    }
}

/**
 * @brief Retrieves the SDL_Texture associated with the given file name.
 *
 * The Textures that are returned by this function are stored in the ImageManager map and are destroyed when the ImageManager is destroyed.
 *
 * @param fileName The name of the file to retrieve the texture for.
 * @return SDL_Texture* The SDL_Texture associated with the file name, or nullptr if not found.
 */
SDL_Texture *ImageManager::GetTexture(const std::string &fileName)
{
    if (fileName.empty())
    {
        SDL_Log("Please provide a file name\n");
        return nullptr;
    }

    if (m_assets.empty())
    {
        SDL_Log("Please call LoadAllImagesFromPath() first.\n");
        return nullptr;
    }

    std::string localFileName = fileName;
    size_t dotPos = localFileName.find('.');
    if (dotPos != std::string::npos)
    {
        localFileName = localFileName.substr(0, dotPos);
    }

    auto it = m_assets.find(localFileName);
    if (it != m_assets.end())
    {
        return static_cast<SDL_Texture *>(it->second);
    }
    else
    {
        SDL_Log("Texture not found: %s\n", localFileName.c_str());
        return nullptr;
    }
}