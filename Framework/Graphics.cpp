#include "Graphics.h"

// Tell linker to bind D3D11 libraries (double check if you already did this in project settings)
#pragma comment(lib, "d3d11.lib")

Graphics::Graphics(HWND hWnd, int width, int height)
{
    // Define configuration for the swap chain (Buffer settings)
    DXGI_SWAP_CHAIN_DESC sd = { 0 };
    sd.BufferDesc.Width = width;
    sd.BufferDesc.Height = height;
    sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; // Standard 32-bit color (RGBA)
    sd.BufferDesc.RefreshRate.Numerator = 0;           // Limitless refresh rate
    sd.BufferDesc.RefreshRate.Denominator = 0;
    sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
    sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
    sd.SampleDesc.Count = 1;                           // No anti-aliasing (MSAA) for now
    sd.SampleDesc.Quality = 0;
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;  // Use buffer to draw on screen
    sd.BufferCount = 1;                                // 1 back buffer + 1 front buffer (Double buffering)
    sd.OutputWindow = hWnd;                            // Target window handle
    sd.Windowed = TRUE;                                // Run in windowed mode
    sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
    sd.Flags = 0;

    // Create Device, Context and SwapChain simultaneously
    D3D11CreateDeviceAndSwapChain(
        nullptr,                  // Default graphics adapter
        D3D_DRIVER_TYPE_HARDWARE, // Use hardware GPU acceleration
        nullptr,                  // No software driver implementation
        0,                        // Runtime flags
        nullptr,                  // Use highest available feature level
        0,
        D3D11_SDK_VERSION,        // Standard SDK version
        &sd,
        &m_swap,
        &m_device,
        nullptr,
        &m_context
    );

    // Get the back buffer texture from the swap chain
    Microsoft::WRL::ComPtr<ID3D11Texture2D> pBackBuffer;
    m_swap->GetBuffer(0, __uuidof(ID3D11Texture2D), &pBackBuffer);

    // Create the Render Target View using the back buffer
    m_device->CreateRenderTargetView(pBackBuffer.Get(), nullptr, &m_target);

    // Set viewport bounds to match the window dimensions
    D3D11_VIEWPORT vp;
    vp.Width = static_cast<float>(width);
    vp.Height = static_cast<float>(height);
    vp.MinDepth = 0.0f;
    vp.MaxDepth = 1.0f;
    vp.TopLeftX = 0.0f;
    vp.TopLeftY = 0.0f;
    m_context->RSSetViewports(1u, &vp);
}

void Graphics::BeginFrame(float r, float g, float b) noexcept
{
    // Clear the screen with a specific background color
    const float color[] = { r, g, b, 1.0f };
    m_context->ClearRenderTargetView(m_target.Get(), color);
}

void Graphics::EndFrame() noexcept
{
    // Present the back buffer to the monitor screen
    // First parameter 1 = Enable VSync (caps frame rate to screen refresh rate)
    m_swap->Present(1u, 0u);
}