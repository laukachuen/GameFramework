#include "App.h"

#include "TitleState.h"

App::App()
    : m_wnd(800, 600, L"My Custom Game Framework", L"../Framework/Shader/BufferShaders.hlsl")
    , m_gameStateMgr(m_wnd.kbd, m_wnd.Gfx())
{
    m_gameStateMgr.RequestStateChange(std::make_unique<TitleState>(m_gameStateMgr.GetContext()));
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
    m_gameStateMgr.Update(dt);
    m_wnd.kbd.Update();
}

void App::Render()
{
    m_wnd.Gfx().BeginFrame(0.0f, 0.0f, 0.0f);
    m_gameStateMgr.Draw();

    m_wnd.Gfx().PresentBuffer(); 
    m_wnd.Gfx().EndFrame();                        
}