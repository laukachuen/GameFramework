#include "Keyboard.h"

bool Keyboard::IsKeyPressed(unsigned char keycode) const noexcept
{
    return m_keystates[keycode];
}

void Keyboard::OnKeyPressed(unsigned char keycode) noexcept
{
    m_keystates[keycode] = true;
}

void Keyboard::OnKeyReleased(unsigned char keycode) noexcept
{
    m_keystates[keycode] = false;
}

void Keyboard::ClearState() noexcept
{
    m_keystates.reset();
}