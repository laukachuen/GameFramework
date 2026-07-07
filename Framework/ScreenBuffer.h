#pragma once
#include <vector>
#include <cstdint>

// brief Represents a CPU-side software frame buffer for pixel manipulation.

class ScreenBuffer
{
public:
    // Initializes the buffer with given dimensions and allocates memory
    ScreenBuffer(int width, int height);
    ~ScreenBuffer() = default;

    // Clears the entire buffer with a specific ARGB color (e.g., 0xAARRGGBB)
    void Clear(uint32_t color);

    // Sets the color of a specific pixel at coordinates (x, y)
    void SetPixel(int x, int y, uint32_t color);

    // Inline getters for dimensions and raw pixel data pointer
    int GetWidth() const { return m_width; }
    int GetHeight() const { return m_height; }
    const uint32_t* GetPixels() const { return m_pixels.data(); }

private:
    int m_width;
    int m_height;
    std::vector<uint32_t> m_pixels; // 1D array representing 2D grid of pixels
};