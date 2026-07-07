#include "Mouse.h"

void Mouse::OnMouseMove(int x, int y) noexcept
{
    m_x = x;
    m_y = y;
}

void Mouse::OnMouseLeave() noexcept
{
    m_bIsInWindow = false;
}

void Mouse::OnMouseEnter() noexcept
{
    m_bIsInWindow = true;
}

void Mouse::OnLeftPressed() noexcept
{
    m_bIsLeftPressed = true;
}

void Mouse::OnLeftReleased() noexcept
{
    m_bIsLeftPressed = false;
}

void Mouse::OnRightPressed() noexcept
{
    m_bIsRightPressed = true;
}

void Mouse::OnRightReleased() noexcept
{
    m_bIsRightPressed = false;
}