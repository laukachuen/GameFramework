#include "Mouse.h"

void Mouse::OnMouseMove(int x, int y) noexcept
{
    m_x = x;
    m_y = y;
}

void Mouse::OnMouseLeave() noexcept
{
    m_isInWindow = false;
}

void Mouse::OnMouseEnter() noexcept
{
    m_isInWindow = true;
}

void Mouse::OnLeftPressed() noexcept
{
    m_isLeftPressed = true;
}

void Mouse::OnLeftReleased() noexcept
{
    m_isLeftPressed = false;
}

void Mouse::OnRightPressed() noexcept
{
    m_isRightPressed = true;
}

void Mouse::OnRightReleased() noexcept
{
    m_isRightPressed = false;
}