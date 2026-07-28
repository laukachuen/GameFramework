#pragma once
#include <cstdint>

class Color
{
public:
	constexpr Color() = default;
	constexpr Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255)
		: m_r(r), m_g(g), m_b(b), m_a(a)
	{}

	constexpr uint8_t GetR() const { return m_r; }
	constexpr uint8_t GetG() const { return m_g; }
	constexpr uint8_t GetB() const { return m_b; }
	constexpr uint8_t GetA() const { return m_a; }

	constexpr uint32_t ToPacked() const
	{
		return
			(static_cast<uint32_t>(m_a) << 24) |
			(static_cast<uint32_t>(m_b) << 16) |
			(static_cast<uint32_t>(m_g) << 8)  |
			 static_cast<uint32_t>(m_r);
	}
	
	static constexpr Color FromPacked(uint32_t packed)
	{
		return Color(
			static_cast<uint8_t>(packed & 0xFF), 
			static_cast<uint8_t>((packed >> 8 ) & 0xFF),
			static_cast<uint8_t>((packed >> 16) & 0xFF),
			static_cast<uint8_t>((packed >> 24) & 0xFF)
		);
	}

	constexpr bool operator==(const Color& other) const
	{
		return
			m_r == other.m_r &&
			m_g == other.m_g &&
			m_b == other.m_b &&
			m_a == other.m_a;
	}

	constexpr bool operator!=(const Color& other) const
	{
		return !(*this == other);
	}

	static constexpr Color Red()
	{
		return Color(255, 0, 0);
	}

	static constexpr Color Green()
	{
		return Color(0, 255, 0);
	}

	static constexpr Color Blue()
	{
		return Color(0, 0, 255);
	}

	static constexpr Color White()
	{
		return Color(255, 255, 255);
	}

	static constexpr Color Black()
	{
		return Color(0, 0, 0);
	}

	static constexpr Color Transparent()
	{
		return Color(0, 0, 0, 0);
	}

private:
	uint8_t m_r = 0;
	uint8_t m_g = 0;
	uint8_t m_b = 0;
	uint8_t m_a = 255;
};
