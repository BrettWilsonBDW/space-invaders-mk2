#include "Utils.hpp"
#include <cmath>
#include <iostream>

void Utils::CalculateScaleFactor(int baseWindowWidth, int baseWindowHeight)
{
    int width, height;
    SDL_GetWindowSize(m_window, &width, &height);

    // Calculate the scale factor based on the base window size
    float widthScaleFactor = static_cast<float>(width) / static_cast<float>(baseWindowWidth);
    float heightScaleFactor = static_cast<float>(height) / static_cast<float>(baseWindowHeight);

    // Use the minimum of the two scale factors to maintain aspect ratio
    m_scaleFactor = fminf(widthScaleFactor, heightScaleFactor);
}