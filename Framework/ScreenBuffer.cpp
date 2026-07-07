#include "ScreenBuffer.h"
#include <algorithm>

ScreenBuffer::ScreenBuffer(int width, int height)
    : m_width(width), m_height(height)
{
    // Pre-allocate memory for the pixel array and default to solid black (0xFF000000)
    size_t totalPixels = static_cast<size_t>(m_width * m_height);
    m_pixels.resize(totalPixels, 0xFF000000);
}

void ScreenBuffer::Clear(uint32_t color)
{
    // Efficiently fill the entire contiguous memory block with the target color
    std::fill(m_pixels.begin(), m_pixels.end(), color);
}

void ScreenBuffer::SetPixel(int x, int y, uint32_t color)
{
    // Boundary check to prevent out-of-bounds memory writes which causes crashes
    if (x < 0 || x >= m_width || y < 0 || y >= m_height)
    {
        return;
    }

    // Convert 2D screen coordinates to 1D flat array index
    int index = y * m_width + x;
    m_pixels[index] = color;
}