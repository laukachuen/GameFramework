#pragma once

class Mouse
{
    friend class Window;

public:
    Mouse() = default;
    ~Mouse() = default;

    // Get current cursor coordinates relative to the window's client area
    int GetX() const noexcept { return m_x; }
    int GetY() const noexcept { return m_y; }

    // Check state of mouse buttons
    bool IsLeftPressed() const noexcept { return m_isLeftPressed; }
    bool IsRightPressed() const noexcept { return m_isRightPressed; }
    bool IsInWindow() const noexcept { return m_isInWindow; }

private:
    // Internal state updates driven by Window messages
    void OnMouseMove(int x, int y) noexcept;
    void OnMouseLeave() noexcept;
    void OnMouseEnter() noexcept;
    void OnLeftPressed() noexcept;
    void OnLeftReleased() noexcept;
    void OnRightPressed() noexcept;
    void OnRightReleased() noexcept;

private:
    int m_x = 0;
    int m_y = 0;
    bool m_isLeftPressed = false;
    bool m_isRightPressed = false;
    bool m_isInWindow = false;
};