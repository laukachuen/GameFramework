#pragma once
#include "Window.h"
#include "Timer.h"
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

    Image m_test;
};