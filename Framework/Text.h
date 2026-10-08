#pragma once
#include "Color.h"
#include <string>
#include <utility>

class FreeTypeFont;
class ScreenBuffer;

class Text
{
public:
	Text(FreeTypeFont& font, const std::string& text, int x, int y, Color color = Color::White());
	~Text() = default;

	Text(const Text&) = delete;
	Text& operator=(const Text&) = delete;

	void Draw(ScreenBuffer& buffer);

	void SetText(const std::string& text);
	void SetPosition(int x, int y);
	void SetColor(Color color);

	const std::string& GetText();
	int GetX() const;
	int GetY() const;
	Color GetColor();

	std::pair<int, int> GetSize() const;

private:
	FreeTypeFont& m_font;
	std::string m_text;
	int m_x;
	int m_y;
	Color m_color;
};

