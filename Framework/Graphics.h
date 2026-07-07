#pragma once
#include <windows.h>
#include <wrl/client.h> // Required for Microsoft::WRL::ComPtr
#include <d3d11.h>

class Graphics
{
public:
    Graphics(HWND hWnd, int width, int height);
    ~Graphics() = default; // ComPtr cleans up automatically

    // Prevent copying
    Graphics(const Graphics&) = delete;
    Graphics& operator=(const Graphics&) = delete;

    // Clear buffer and swap screen
    void BeginFrame(float r, float g, float b) noexcept;
    void EndFrame() noexcept;

private:
    // Smart pointers for DirectX 11 interfaces
    Microsoft::WRL::ComPtr<ID3D11Device>            m_pDevice;
    Microsoft::WRL::ComPtr<ID3D11DeviceContext>     m_pContext;
    Microsoft::WRL::ComPtr<IDXGISwapChain>          m_pSwap;
    Microsoft::WRL::ComPtr<ID3D11RenderTargetView>  m_pTarget;
};