#pragma once
#include <vector>
#include <cstdint>
#include "Image.h"
#include "Color.h"

// brief Represents a CPU-side software frame buffer for pixel manipulation.

class ScreenBuffer
{
public:
    // Initializes the buffer with given dimensions and allocates memory
    ScreenBuffer(int width, int height);
    ~ScreenBuffer() = default;

    // Clears the entire buffer with a specific ARGB color (e.g., 0xAARRGGBB)
    void Clear(Color color);

    // Sets the color of a specific pixel at coordinates (x, y)
    void SetPixel(int x, int y, Color color);

    // Inline getters for dimensions and raw pixel data pointer
    const Color* GetPixels() const;
    int GetWidth() const;
    int GetHeight() const;

    void DrawImage(const Image& image, int destX, int destY);


private:
    int m_width;
    int m_height;
    std::vector<Color> m_pixels; // 1D array representing 2D grid of pixels
};