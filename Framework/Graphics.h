#pragma once
#include <d3d11.h>
#include <d3dcompiler.h>
#include <wrl/client.h>
#include <memory>
#include <string>
#include "ScreenBuffer.h"

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")

class Graphics
{
public:
    Graphics(HWND hWnd, int width, int height, const std::wstring& shaderPath); // Pass the shader file path during initialization
    ~Graphics() = default; // ComPtr automatically handles resource cleanup (RAII)

    // Disallow copying to prevent resource duplication issues
    Graphics(const Graphics&) = delete;
    Graphics& operator=(const Graphics&) = delete;

    void BeginFrame(float r, float g, float b);
    void EndFrame();

    // Present the software screen buffer to the GPU texture and render it
    void PresentBuffer(const ScreenBuffer& buffer);

private:
    // Helper function to compile and initialize shaders and input layout
    bool InitializeShaders();

    // Helper function to create the full-screen quad vertex buffer
    bool InitializeQuad();

private:
    std::wstring m_shaderPath;

private:
    // Core DirectX 11 Interfaces wrapped in ComPtr
    Microsoft::WRL::ComPtr<ID3D11Device>           m_device;
    Microsoft::WRL::ComPtr<ID3D11DeviceContext>    m_context;
    Microsoft::WRL::ComPtr<IDXGISwapChain>         m_swapChain;
    Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_renderTargetView;

    // Software Rendering Pipeline Resources wrapped in ComPtr
    Microsoft::WRL::ComPtr<ID3D11VertexShader>     m_vertexShader;
    Microsoft::WRL::ComPtr<ID3D11PixelShader>      m_pixelShader;
    Microsoft::WRL::ComPtr<ID3D11InputLayout>      m_inputLayout;
    Microsoft::WRL::ComPtr<ID3D11Buffer>           m_vertexBuffer;

    // Texture resources to hold the CPU pixel data
    Microsoft::WRL::ComPtr<ID3D11Texture2D>        m_dynamicTexture;
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_textureSRV;
    Microsoft::WRL::ComPtr<ID3D11SamplerState>     m_samplerState;
};