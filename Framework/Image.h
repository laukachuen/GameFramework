#pragma once
#include <string>
#include <vector>
#include <cstdint>
#include "Color.h"

#pragma pack(push, 1)
struct BMPFileHeader 
{
    uint16_t bfType;          // File type, must be 0x4D42 ("BM")
    uint32_t bfSize;          // Size of the BMP file in bytes
    uint16_t bfReserved1;     // Reserved, must be 0
    uint16_t bfReserved2;     // Reserved, must be 0
    uint32_t bfOffBits;       // Offset to start of pixel data
};

struct BMPInfoHeader 
{
    uint32_t biSize;          // Size of this header (40 bytes)
    int32_t  biWidth;         // Image width in pixels
    int32_t  biHeight;        // Image height in pixels (negative = top-down, positive = bottom-up)
    uint16_t biPlanes;        // Number of color planes (must be 1)
    uint16_t biBitCount;      // Bits per pixel (24 for RGB, 32 for RGBA)
    uint32_t biCompression;   // Compression type (0 = BI_RGB, 3 = BI_BITFIELDS)
    uint32_t biSizeImage;     // Image size in bytes (can be 0 for uncompressed)
    int32_t  biXPelsPerMeter; // Horizontal pixels per meter
    int32_t  biYPelsPerMeter; // Vertical pixels per meter
    uint32_t biClrUsed;       // Number of colors in color palette
    uint32_t biClrImportant;  // Number of important colors
};
#pragma pack(pop)

class Image
{
public:
    Image() = default;

    bool LoadBMP(const std::string& filepath);

    Color GetPixel(int x, int y) const;
    int GetWidth() const;
    int GetHeight() const;

private:
    int m_width = 0;
    int m_height = 0;
    std::vector<Color> m_pixels;
};