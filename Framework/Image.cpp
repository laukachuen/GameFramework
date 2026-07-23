#include "Image.h"
#include <windows.h>
#include <fstream>
#include <cmath>

bool Image::LoadBMP(const std::string& filepath)
{
    OutputDebugStringA("\n=====================================\n");
    OutputDebugStringA(">>> [BMP Debug] LoadBMP Started! <<<\n");
    OutputDebugStringA("=====================================\n\n");

    std::ifstream file(filepath, std::ios::binary);
    if (!file)
    {
        OutputDebugStringA("[BMP Error] File NOT found!\n");
        return false;
    }

    BMPFileHeader fileHeader{};
    BMPInfoHeader infoHeader{};

    file.read(reinterpret_cast<char*>(&fileHeader), sizeof(fileHeader));
    file.read(reinterpret_cast<char*>(&infoHeader), sizeof(infoHeader));

    // Check Magic Number (0x4D42 represents 'BM')
    if (fileHeader.bfType != 0x4D42)
    {
        OutputDebugStringA("[BMP Error] Invalid BMP Magic Number!\n");
        return false;
    }

    // Compression check: Accept 0 (BI_RGB) and 3 (BI_BITFIELDS, commonly exported by Aseprite)
    if (infoHeader.biCompression != 0 && infoHeader.biCompression != 3)
    {
        OutputDebugStringA("[BMP Error] Unsupported compression format!\n");
        return false;
    }

    // Bit depth check: Only support 24-bit (RGB) and 32-bit (RGBA)
    if (infoHeader.biBitCount != 24 && infoHeader.biBitCount != 32)
    {
        OutputDebugStringA("[BMP Error] Unsupported bit count!\n");
        return false;
    }

    m_width = infoHeader.biWidth;
    m_height = std::abs(infoHeader.biHeight); 
    m_pixels.resize(m_width * m_height);

    char debugBuf[512];
    sprintf_s(debugBuf,
        "[BMP Header Check]\n"
        "bfType (0x4D42 'BM') : 0x%X\n"
        "bfOffBits            : %u\n"
        "biWidth x Height     : %d x %d\n"
        "biBitCount           : %u\n"
        "biCompression        : %u\n\n",
        fileHeader.bfType,
        fileHeader.bfOffBits,
        m_width, m_height,
        infoHeader.biBitCount,
        infoHeader.biCompression
    );
    OutputDebugStringA(debugBuf);

    // Jump directly to the start of pixel data (e.g., offset byte 70)
    file.seekg(fileHeader.bfOffBits, std::ios::beg);

    // If biHeight is negative, image is stored top-down; otherwise bottom-up
    const bool topDown = (infoHeader.biHeight < 0);
    const bool is32Bit = (infoHeader.biBitCount == 32);

    // Each row in 24-bit BMP must be padded to a multiple of 4 bytes
    const int padding = is32Bit ? 0 : (4 - (m_width * 3) % 4) % 4;

    for (int row = 0; row < m_height; ++row)
    {
        // Flip row index vertically for bottom-up BMPs to avoid upside-down texture
        int y = topDown ? row : (m_height - 1 - row);

        for (int x = 0; x < m_width; ++x)
        {
            // BMP stores pixels in BGR(A) order
            uint8_t b = file.get();
            uint8_t g = file.get();
            uint8_t r = file.get();
            uint8_t a = 255;

            if (is32Bit)
            {
                a = file.get();
                // Defensive fallback: If alpha is 0 (fully transparent), force it to 255 (opaque)
                if (a == 0)
                {
                    a = 255;
                }
            }

            // Combine components into 0xAARRGGBB format and store into vector
            m_pixels[y * m_width + x] =
                (static_cast<uint32_t>(a) << 24) |
                (static_cast<uint32_t>(r) << 16) |
                (static_cast<uint32_t>(g) << 8) |
                static_cast<uint32_t>(b);
        }

        // Skip padding bytes at the end of each row for 24-bit images
        if (!is32Bit && padding > 0)
        {
            file.seekg(padding, std::ios::cur);
        }
    }

    OutputDebugStringA("\n===================================================\n");
    OutputDebugStringA(">>> [BMP Success] Texture loaded successfully! <<<\n");
    OutputDebugStringA("===================================================\n\n");
    return true;
}

uint32_t Image::GetPixel(int x, int y) const 
{ 
    return m_pixels[y * m_width + x]; 
}

int Image::GetWidth() const 
{ 
    return m_width; 
}

int Image::GetHeight() const 
{ 
    return m_height; 
}
