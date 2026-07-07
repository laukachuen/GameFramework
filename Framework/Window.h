#pragma once
#include <Windows.h>
#include <string>
#include <memory>

#include "Keyboard.h"
#include "Mouse.h"
#include "Graphics.h"


class Window
{
public:
    Window(int width, int height, const std::wstring& title);
    ~Window();

    // Prevent copying of the window instance to avoid multiple window bugs
    Window(const Window&) = delete;
    Window& operator=(const Window&) = delete;

    // Process Windows OS messages. Returns false if WM_QUIT is received.
    bool ProcessMessages();

    // Getter for the window handle (DirectX 11 will need this later)
    HWND GetHWND() const { return m_hWnd; }

public:
    // Public instances so game loop can do: window.kbd.IsKeyPressed(...)
    Keyboard kbd;
    Mouse mouse;

    // Getter to access graphics from outside
    Graphics& Gfx() { return *m_pGfx; }

private:
    // Setup static message handler (used only to catch the very first setup messages)
    static LRESULT CALLBACK HandleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
    // Main static message handler that redirects OS messages to our C++ class instance
    static LRESULT CALLBACK HandleMsgThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
    // Member function that actually processes messages for this specific window instance
    LRESULT HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;

private:
    int m_width;
    int m_height;
    HWND m_hWnd = nullptr;
    HINSTANCE m_hInstance;
    std::wstring m_windowClassName = L"MyGameWindowDeviceClass";

private:
    // std::unique_ptr manages our custom Graphics class lifecycle object
    std::unique_ptr<Graphics> m_pGfx;
};