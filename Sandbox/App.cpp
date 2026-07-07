#include "App.h"

App::App()
    : m_wnd(800, 600, L"My Custom Game Framework", L"../Framework/Shader/BufferShaders.hlsl")
{
    // Instantiate the software screen buffer matching the exact window resolution
    m_screenBuffer = std::make_unique<ScreenBuffer>(800, 600);
}

int App::Run()
{
    while (true)
    {
        // 1. Process all Windows OS messages first
        if (!m_wnd.ProcessMessages())
        {
            // If WM_QUIT is received, exit the loop with standard exit code
            return 0;
        }

        // 2. Get the exact elapsed time since the last frame (deltaTime)
        float dt = m_timer.Mark();

        // 3. Update game simulation and then render graphics
        Update(dt);
        Render();
    }
}

void App::Update(float dt)
{
    // ====== GAME PLAY LOGIC ======

    // Move the testing green line across the screen based on time (150 pixels per second)
    m_lineX += 150.0f * dt;
    if (m_lineX >= m_screenBuffer->GetWidth())
    {
        m_lineX = 0.0f;
    }

    // Example: Test input using our window's input components
    if (m_wnd.kbd.IsKeyPressed('W'))
    {
        // Player moves forward based on time (dt), not frame rate!
    }
}

void App::Render()
{
    // ====== GRAPHICS RENDERING ======

    // 1. Clear the CPU software buffer with a dark retro charcoal color (0xFF1A1A1A)
    m_screenBuffer->Clear(0xFF1A1A1A);

    // 2. Draw a moving vertical green line to test real-time CPU frame updates
    int currentLineX = static_cast<int>(m_lineX);
    for (int y = 0; y < m_screenBuffer->GetHeight(); ++y)
    {
        m_screenBuffer->SetPixel(currentLineX, y, 0xFF00FF00); // Solid green pixel
    }

    // 3. Draw a static 100x100 retro checkerboard pattern in the center of the screen
    int centerX = m_screenBuffer->GetWidth() / 2;
    int centerY = m_screenBuffer->GetHeight() / 2;

    for (int y = centerY - 50; y < centerY + 50; ++y)
    {
        for (int x = centerX - 50; x < centerX + 50; ++x)
        {
            // Create an 8x8 pixel tile grid toggle condition
            if (((x / 8) + (y / 8)) % 2 == 0)
            {
                m_screenBuffer->SetPixel(x, y, 0xFFFF00FF); // Magenta tile
            }
            else
            {
                m_screenBuffer->SetPixel(x, y, 0xFF00FFFF); // Cyan tile
            }
        }
    }

    // 4. Clear DX11 Backbuffer, copy CPU pixels to GPU texture, and present
    m_wnd.Gfx().BeginFrame(0.0f, 0.0f, 0.0f);       // Clear render target view to black
    m_wnd.Gfx().PresentBuffer(*m_screenBuffer);    // Upload CPU canvas & render via full-screen quad HLSL
    m_wnd.Gfx().EndFrame();                         // Flip front/back buffers (V-Sync)
}