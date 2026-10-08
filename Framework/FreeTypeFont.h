#pragma once
#include "ScreenBuffer.h"
#include "Color.h"
#include <string>
#include <utility>

#include <ft2build.h>
#include FT_FREETYPE_H

class FreeTypeFont {
public:
    // Initialize FreeType and load font face
    FreeTypeFont(const std::string& fontPath, int pixelSize);
    ~FreeTypeFont();

    // Renders a string onto a provided Surface
    void RenderStringToBuffer(const std::string& text, int startX, int startY, Color color, ScreenBuffer& buffer);

    // Returns exact { width, height } needed for the string
    std::pair <int, int> CalculateStringSize(const std::string& text);

private:
    FT_Library m_ftLibrary = nullptr;
    FT_Face m_ftFace = nullptr;
};