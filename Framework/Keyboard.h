#pragma once
#include <bitset>

class Keyboard
{
    // Let Window access private states directly to update them via Windows Messages
    friend class Window;

public:
    Keyboard() = default;
    ~Keyboard() = default;

    // Core API for game logic to check if a specific key is currently held down
    bool IsKeyPressed(unsigned char keycode) const noexcept;

private:
    // Functions called internally by Window class when OS messages arrive
    void OnKeyPressed(unsigned char keycode) noexcept;
    void OnKeyReleased(unsigned char keycode) noexcept;
    void ClearState() noexcept;

private:
    // A bitset tracking all 256 possible virtual key codes (VK_...)
    std::bitset<256> m_keystates;
};