#include "App.h"

App::App()
    : m_wnd(800, 600, L"My Custom Game Framework", L"../Framework/Shader/BufferShaders.hlsl")
{
    m_test.LoadBMP("Assets/Test.bmp");
}

int App::Run()
{
    while (true)
    {
        // Process all Windows OS messages first
        if (!m_wnd.ProcessMessages())
        {
            // If WM_QUIT is received, exit the loop with standard exit code
            return 0;
        }

        // Get the exact elapsed time since the last frame (deltaTime)
        float dt = m_timer.Mark();

        // Update game simulation and then render graphics
        Update(dt);
        Render();
    }
}

void App::Update(float dt)
{
    
}

void App::Render()
{
    m_wnd.Gfx().GetScreenBuffer().Clear(Color::Black());
    m_wnd.Gfx().GetScreenBuffer().DrawImage(m_test, 100, 100);

    m_wnd.Gfx().BeginFrame(0.0f, 0.0f, 0.0f);
    m_wnd.Gfx().PresentBuffer(); 
    m_wnd.Gfx().EndFrame();                        
}