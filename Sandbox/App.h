#pragma once
#include "Window.h"
#include "Timer.h"
#include "ScreenBuffer.h" 
#include <memory>
#include "Image.h"

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
    Window m_wnd;                                 
    Timer m_timer;                                
    std::unique_ptr<ScreenBuffer> m_screenBuffer;

    Image m_test;
};