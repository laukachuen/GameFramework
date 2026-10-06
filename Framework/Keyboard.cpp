#include "Keyboard.h"

void Keyboard::Update() noexcept
{
    m_prevKeystates = m_currKeystates;
}

bool Keyboard::IsKeyHeld(unsigned char keycode) const noexcept
{
    return m_currKeystates[keycode];
}

bool Keyboard::IsKeyTriggered(unsigned char keycode) const noexcept
{
    return m_currKeystates[keycode] && !m_prevKeystates[keycode];
}

bool Keyboard::IsKeyReleased(unsigned char keycode) const noexcept
{
    return !m_currKeystates[keycode] && m_prevKeystates[keycode];
}

void Keyboard::OnKeyPressed(unsigned char keycode) noexcept
{
    m_currKeystates[keycode] = true;
}

void Keyboard::OnKeyReleased(unsigned char keycode) noexcept
{
    m_currKeystates[keycode] = false;
}

void Keyboard::ClearState() noexcept
{
    m_currKeystates.reset();
    m_prevKeystates.reset();
}