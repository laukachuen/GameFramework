#include "Image.h"

#include <Windows.h>
#include <fstream>

Image::Image(int width, int height)
    : m_width(width),
    m_height(height),
    m_pixels(width* height)
{
}

bool Image::LoadBMP(const std::string& filepath)
{
    std::ifstream file(filepath, std::ios::binary);
    if (!file)
        return false;

    BITMAPFILEHEADER fileHeader{};
    BITMAPINFOHEADER infoHeader{};

    file.read(reinterpret_cast<char*>(&fileHeader), sizeof(fileHeader));
    file.read(reinterpret_cast<char*>(&infoHeader), sizeof(infoHeader));

    if (infoHeader.biCompression != BI_RGB)
        return false;

    if (infoHeader.biBitCount != 24 &&
        infoHeader.biBitCount != 32)
        return false;

    m_width = infoHeader.biWidth;
    m_height = std::abs(infoHeader.biHeight);

    m_pixels.resize(m_width * m_height);

    file.seekg(fileHeader.bfOffBits);

    const bool topDown = infoHeader.biHeight < 0;
    const bool is32Bit = infoHeader.biBitCount == 32;

    const int bytesPerPixel = is32Bit ? 4 : 3;
    const int padding = is32Bit ? 0 : (4 - (m_width * 3) % 4) % 4;

    for (int row = 0; row < m_height; ++row)
    {
        int y = topDown ? row : (m_height - 1 - row);

        for (int x = 0; x < m_width; ++x)
        {
            uint8_t b = file.get();
            uint8_t g = file.get();
            uint8_t r = file.get();

            uint8_t a = 255;

            if (is32Bit)
            {
                a = file.get();
            }

            m_pixels[y * m_width + x] =
                (uint32_t(a) << 24) |
                (uint32_t(r) << 16) |
                (uint32_t(g) << 8) |
                uint32_t(b);
        }

        if (!is32Bit)
        {
            file.seekg(padding, std::ios::cur);
        }
    }

    return true;
}

uint32_t Image::GetPixel(int x, int y) const
{
    if (x < 0 || x >= m_width ||
        y < 0 || y >= m_height)
    {
        return 0xFF000000;
    }

    return m_pixels[y * m_width + x];
}

void Image::SetPixel(int x, int y, uint32_t color)
{
    if (x < 0 || x >= m_width ||
        y < 0 || y >= m_height)
    {
        return;
    }

    m_pixels[y * m_width + x] = color;
}