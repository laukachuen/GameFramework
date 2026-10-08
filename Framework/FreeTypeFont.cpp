#include "FreeTypeFont.h"
#include <stdexcept>

FreeTypeFont::FreeTypeFont(const std::string& fontPath, int pixelSize) 
{
    // Initialize the FreeType library instance
    if (FT_Init_FreeType(&m_ftLibrary))
    {
        throw std::runtime_error("FreeType Init Failed");
    }

    // Load the specific font file into a "Face" object
    if (FT_New_Face(m_ftLibrary, fontPath.c_str(), 0, &m_ftFace))
    {
        throw std::runtime_error("Font Load Failed");
    }

    // Set the character size in pixels (Width 0 = auto-calculate based on height)
    FT_Set_Pixel_Sizes(m_ftFace, 0, pixelSize);
}

FreeTypeFont::~FreeTypeFont() 
{
    if (m_ftFace) 
    {
        FT_Done_Face(m_ftFace);
        m_ftFace = nullptr;
    }

    if (m_ftLibrary) 
    {
        FT_Done_FreeType(m_ftLibrary);
        m_ftLibrary = nullptr;
    }
}

std::pair<int, int> FreeTypeFont::CalculateStringSize(const std::string& text) 
{
    int totalWidth = 0;

    // Calculate total height: Ascender (top to baseline) - Descender (baseline to bottom)
    // Values are in 26.6 fixed-point, so we shift right by 6 bits (divide by 64) to get pixels
    int totalHeight = (m_ftFace->size->metrics.ascender - m_ftFace->size->metrics.descender) >> 6;

    for (char c : text) 
    {
        // Load glyph to get its advance width
        if (FT_Load_Char(m_ftFace, c, FT_LOAD_DEFAULT))
        {
            continue;
        }

        // Advance.x is the distance to the next character's starting point
        totalWidth += (m_ftFace->glyph->advance.x >> 6);
    }
    return { totalWidth, totalHeight };
}

void FreeTypeFont::RenderStringToBuffer(const std::string& text, int startX, int startY, Color color, ScreenBuffer& buffer)
{
    int xOffset = startX;
    // Get the font's ascender to align all characters to a consistent baseline
    int ascender = m_ftFace->size->metrics.ascender >> 6;

    for (char c : text) 
    {
        // Load and render the glyph into a monochrome bitmap
        if (FT_Load_Char(m_ftFace, c, FT_LOAD_RENDER)) continue;

        auto& bmp = m_ftFace->glyph->bitmap;

        // Horizontal and vertical position relative to the cursor/baseline
        int bLeft = m_ftFace->glyph->bitmap_left;
        int bTop = m_ftFace->glyph->bitmap_top;

        // Iterate through the rendered bitmap pixels
        for (int y = 0; y < (int)bmp.rows; y++) 
        {
            for (int x = 0; x < (int)bmp.width; x++) 
            {
                // Anti-aliasing
                unsigned char alpha = bmp.buffer[y * bmp.width + x];

                // Only draw if the pixel has some opacity
                if (alpha > 0) 
                {
                    // xOffset: cursor position, bX: glyph padding, x: current pixel in bitmap
                    int drawX = xOffset + bLeft + x;
                    // Align the glyph based on the font's ascender line
                    int drawY = startY + (ascender - bTop + y);

                    // Bounds checking to prevent drawing outside the target Surface
                    if (drawX >= 0 && drawX < (int)buffer.GetWidth() &&
                        drawY >= 0 && drawY < (int)buffer.GetHeight())
                    {
                        // Set pixel with the font's alpha value (White color with variable transparency)
                        buffer.SetPixel(drawX, drawY, color);
                    }
                }
            }
        }
        // Move the cursor forward for the next character
        xOffset += (m_ftFace->glyph->advance.x >> 6);
    }
}