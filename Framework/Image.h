#pragma once

#include <string>
#include <vector>
#include <cstdint>

class Image
{
public:
    Image() = default;
    Image(int width, int height);

    bool LoadBMP(const std::string& filepath);

    uint32_t GetPixel(int x, int y) const;
    void SetPixel(int x, int y, uint32_t color);

    int GetWidth() const { return m_width; }
    int GetHeight() const { return m_height; }

private:
    int m_width = 0;
    int m_height = 0;
    std::vector<uint32_t> m_pixels;
};