#pragma once
#include "Window.h"
#include "Timer.h"
#include "ScreenBuffer.h" // Include our new CPU software canvas
#include <memory>

class App
{
public:
    App();
    ~App() = default;

    // Prevent copying to maintain strict resource ownership
    App(const App&) = delete;
    App& operator=(const App&) = delete;

    // The actual infinite loop that runs the game
    int Run();

private:
    // Core game loop stages
    void Update(float dt);
    void Render();

private:
    Window m_wnd;                                 // Owns native window handle and Gfx()
    Timer m_timer;                                // Measures frame delta time
    std::unique_ptr<ScreenBuffer> m_screenBuffer; // CPU side pixel buffer for software rendering

    float m_lineX = 0.0f;                         // Test variable for real-time pixel animation
};