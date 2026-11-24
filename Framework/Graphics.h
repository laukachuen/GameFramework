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
    Graphics(HWND hWnd, int width, int height, const std::wstring& shaderDirectory); // Pass the shader file path during initialization
    ~Graphics() = default; // ComPtr automatically handles resource cleanup (RAII)

    // Disallow copying to prevent resource duplication issues
    Graphics(const Graphics&) = delete;
    Graphics& operator=(const Graphics&) = delete;

    void BeginFrame(float r, float g, float b);
    void EndFrame();

    ScreenBuffer& GetScreenBuffer();
    const ScreenBuffer& GetScreenBuffer() const;

    void PresentBuffer();
    void DrawTriangle();

private:
    // Helper function to compile and initialize shaders and input layout
    bool InitializeShaders();

    // Helper function to create the full-screen quad vertex buffer
    bool InitializeQuad();

    // GPU Hardware Rendering
    bool InitializeHardwareShaders();
    bool InitializeTriangle();
    bool InitializeConstantBuffer();

private:
    std::wstring m_shaderDirectory;
    std::unique_ptr<ScreenBuffer> m_screenBuffer;

    struct BufferVertex
    {
        float x, y, z, w; // Position in clip space
        float u, v;       // Texture coordinates (UV)
    };

    struct Vertex
    {
        float x, y, z;
        float r, g, b;
    };

    struct ConstantBuffer
    {
        float offsetX;
        float offsetY;
        float padding[2];
    };

private:
    // Core DirectX 11 Interfaces wrapped in ComPtr
    Microsoft::WRL::ComPtr<ID3D11Device>           m_device;
    Microsoft::WRL::ComPtr<ID3D11DeviceContext>    m_deviceContext;
    Microsoft::WRL::ComPtr<IDXGISwapChain>         m_swapChain;
    Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_renderTargetView;

    // Software Rendering Pipeline Resources wrapped in ComPtr
    Microsoft::WRL::ComPtr<ID3D11VertexShader>     m_bufferVertexShader;
    Microsoft::WRL::ComPtr<ID3D11PixelShader>      m_bufferPixelShader;
    Microsoft::WRL::ComPtr<ID3D11InputLayout>      m_bufferInputLayout;
    Microsoft::WRL::ComPtr<ID3D11Buffer>           m_bufferVertexBuffer;

    // GPU Hardware Rendering
    Microsoft::WRL::ComPtr<ID3D11VertexShader>     m_vertexShader;
    Microsoft::WRL::ComPtr<ID3D11PixelShader>      m_pixelShader;
    Microsoft::WRL::ComPtr<ID3D11InputLayout>      m_inputLayout;
    Microsoft::WRL::ComPtr<ID3D11Buffer>           m_vertexBuffer;
    Microsoft::WRL::ComPtr<ID3D11Buffer>           m_constantBuffer;

    // Texture resources to hold the CPU pixel data
    Microsoft::WRL::ComPtr<ID3D11Texture2D>        m_dynamicTexture;
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_textureSRV;
    Microsoft::WRL::ComPtr<ID3D11SamplerState>     m_samplerState;
};