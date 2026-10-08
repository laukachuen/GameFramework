#include "Text.h"
#include "FreeTypeFont.h"

Text::Text(FreeTypeFont& font, const std::string& text, int x, int y, Color color)
	: m_font(font)
	, m_text(text)
	, m_x(x)
	, m_y(y)
	, m_color(color)
{}

void Text::Draw(ScreenBuffer& buffer)
{
	m_font.RenderStringToBuffer(m_text, m_x, m_y, m_color, buffer);
}

void Text::SetText(const std::string& text)
{
	m_text = text;
}

void Text::SetPosition(int x, int y)
{
	m_x = x; m_y = y;
}

void Text::SetColor(Color color)
{
	m_color = color;
}

const std::string& Text::GetText()
{
	return m_text;
}

int Text::GetX() const
{
	return m_x;
}

int Text::GetY() const
{
	return m_y;
}

Color Text::GetColor()
{
	return m_color;
}

std::pair<int, int> Text::GetSize() const
{
	return m_font.CalculateStringSize(m_text);
}