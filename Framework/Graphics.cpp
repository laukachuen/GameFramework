#include "Graphics.h"
#include <iostream>

// Vertex structure matching the VS_INPUT layout in HLSL
struct Vertex
{
    float x, y, z, w; // Position in clip space
    float u, v;       // Texture coordinates (UV)
};

Graphics::Graphics(HWND hWnd, int width, int height)
{
    // =========================================================================
    // 1. Core DX11 Initialization (Device, Context, SwapChain, RTV)
    // =========================================================================

    DXGI_SWAP_CHAIN_DESC scd = {};
    scd.BufferCount = 1;                                    // Double buffering
    scd.BufferDesc.Width = width;
    scd.BufferDesc.Height = height;
    scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;     // Standard 32-bit color
    scd.BufferDesc.RefreshRate.Numerator = 60;
    scd.BufferDesc.RefreshRate.Denominator = 1;
    scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    scd.OutputWindow = hWnd;
    scd.SampleDesc.Count = 1;                               // No multi-sampling
    scd.SampleDesc.Quality = 0;
    scd.Windowed = TRUE;

    // Create Device, Device Context, and Swap Chain using ComPtr addresses
    HRESULT hr = D3D11CreateDeviceAndSwapChain(
        nullptr,
        D3D_DRIVER_TYPE_HARDWARE,
        nullptr,
        0,
        nullptr,
        0,
        D3D11_SDK_VERSION,
        &scd,
        m_swapChain.GetAddressOf(),
        m_device.GetAddressOf(),
        nullptr,
        m_context.GetAddressOf()
    );

    if (FAILED(hr))
    {
        MessageBox(hWnd, L"Failed to create DX11 Device and SwapChain.", L"Error", MB_OK);
        return;
    }

    // Get the back buffer from the swap chain
    Microsoft::WRL::ComPtr<ID3D11Texture2D> backBuffer;
    m_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(backBuffer.GetAddressOf()));

    // Create Render Target View
    m_device->CreateRenderTargetView(backBuffer.Get(), nullptr, m_renderTargetView.GetAddressOf());

    // Set up the Viewport
    D3D11_VIEWPORT vp = {};
    vp.Width = static_cast<float>(width);
    vp.Height = static_cast<float>(height);
    vp.MinDepth = 0.0f;
    vp.MaxDepth = 1.0f;
    vp.TopLeftX = 0.0f;
    vp.TopLeftY = 0.0f;
    m_context->RSSetViewports(1, &vp);

    // =========================================================================
    // 2. Initialize Shaders and Full-Screen Quad Pipeline
    // =========================================================================
    if (!InitializeShaders())
    {
        MessageBox(hWnd, L"Failed to initialize HLSL shaders.", L"Error", MB_OK);
    }

    if (!InitializeQuad())
    {
        MessageBox(hWnd, L"Failed to initialize fullscreen quad vertex buffer.", L"Error", MB_OK);
    }

    // =========================================================================
    // 3. Create Dynamic Texture for CPU Screen Buffer
    // =========================================================================
    D3D11_TEXTURE2D_DESC texDesc = {};
    texDesc.Width = width;
    texDesc.Height = height;
    texDesc.MipLevels = 1;
    texDesc.ArraySize = 1;
    texDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;     // Match uint32_t ARGB format
    texDesc.SampleDesc.Count = 1;
    texDesc.SampleDesc.Quality = 0;
    texDesc.Usage = D3D11_USAGE_DYNAMIC;             // Allows CPU to overwrite memory every frame
    texDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
    texDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE; // Grants write access to CPU
    texDesc.MiscFlags = 0;

    m_device->CreateTexture2D(&texDesc, nullptr, m_dynamicTexture.GetAddressOf());
    m_device->CreateShaderResourceView(m_dynamicTexture.Get(), nullptr, m_textureSRV.GetAddressOf());

    // =========================================================================
    // 4. Create Texture Sampler State (Retro Pixel Filtering)
    // =========================================================================
    D3D11_SAMPLER_DESC sampDesc = {};
    sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT; // Point filtering keeps pixels crisp/sharp for retro look
    sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
    sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
    sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
    sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
    sampDesc.MinLOD = 0;
    sampDesc.MaxLOD = D3D11_FLOAT32_MAX;

    m_device->CreateSamplerState(&sampDesc, m_samplerState.GetAddressOf());
}

bool Graphics::InitializeShaders()
{
    Microsoft::WRL::ComPtr<ID3DBlob> vsBlob;
    Microsoft::WRL::ComPtr<ID3DBlob> psBlob;
    Microsoft::WRL::ComPtr<ID3DBlob> errorBlob;

    // Compile Vertex Shader from file at runtime
    HRESULT hr = D3DCompileFromFile(
        L"BufferShaders.hlsl", nullptr, nullptr,
        "VS_Main", "vs_5_0", 0, 0, vsBlob.GetAddressOf(), errorBlob.GetAddressOf()
    );
    if (FAILED(hr))
    {
        return false;
    }

    // Compile Pixel Shader from file at runtime
    hr = D3DCompileFromFile(
        L"BufferShaders.hlsl", nullptr, nullptr,
        "PS_Main", "ps_5_0", 0, 0, psBlob.GetAddressOf(), errorBlob.GetAddressOf()
    );
    if (FAILED(hr))
    {
        return false;
    }

    // Create shader objects using ComPtr
    m_device->CreateVertexShader(vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(), nullptr, m_vertexShader.GetAddressOf());
    m_device->CreatePixelShader(psBlob->GetBufferPointer(), psBlob->GetBufferSize(), nullptr, m_pixelShader.GetAddressOf());

    // Define input layout required for the fullscreen quad geometry
    D3D11_INPUT_ELEMENT_DESC layout[] =
    {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 16, D3D11_INPUT_PER_VERTEX_DATA, 0 }
    };

    m_device->CreateInputLayout(layout, 2, vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(), m_inputLayout.GetAddressOf());

    return true;
}

bool Graphics::InitializeQuad()
{
    // Quad coordinates spanning across entire Normalized Device Coordinates (NDC: -1 to 1)
    Vertex vertices[] =
    {
        // Position (X, Y, Z, W)           // UV Coordinates (U, V)
        { -1.0f,  1.0f, 0.0f, 1.0f,        0.0f, 0.0f }, // Top-Left
        {  1.0f,  1.0f, 0.0f, 1.0f,        1.0f, 0.0f }, // Top-Right
        { -1.0f, -1.0f, 0.0f, 1.0f,        0.0f, 1.0f }, // Bottom-Left
        {  1.0f, -1.0f, 0.0f, 1.0f,        1.0f, 1.0f }  // Bottom-Right
    };

    D3D11_BUFFER_DESC bd = {};
    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.ByteWidth = sizeof(vertices);
    bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;

    D3D11_SUBRESOURCE_DATA initData = {};
    initData.pSysMem = vertices;

    HRESULT hr = m_device->CreateBuffer(&bd, &initData, m_vertexBuffer.GetAddressOf());
    return SUCCEEDED(hr);
}

void Graphics::PresentBuffer(const ScreenBuffer& buffer)
{
    // 1. Map the dynamic GPU texture memory to upload fresh CPU pixel arrays
    D3D11_MAPPED_SUBRESOURCE mappedResource;
    HRESULT hr = m_context->Map(m_dynamicTexture.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
    if (SUCCEEDED(hr))
    {
        uint8_t* gpuBuffer = reinterpret_cast<uint8_t*>(mappedResource.pData);
        const uint32_t* cpuPixels = buffer.GetPixels();

        // Safe memcpy accounting for texture row pitch/stride layout
        for (int y = 0; y < buffer.GetHeight(); ++y)
        {
            memcpy(
                gpuBuffer + (y * mappedResource.RowPitch),
                cpuPixels + (y * buffer.GetWidth()),
                buffer.GetWidth() * sizeof(uint32_t)
            );
        }
        m_context->Unmap(m_dynamicTexture.Get(), 0);
    }

    // 2. Set up Input Assembler and pipeline stages to render the quad
    UINT stride = sizeof(Vertex);
    UINT offset = 0;
    m_context->IASetVertexBuffers(0, 1, m_vertexBuffer.GetAddressOf(), &stride, &offset);
    m_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
    m_context->IASetInputLayout(m_inputLayout.Get());

    m_context->VSSetShader(m_vertexShader.Get(), nullptr, 0);
    m_context->PSSetShader(m_pixelShader.Get(), nullptr, 0);

    // Bind textures and filters to the Pixel Shader stage
    m_context->PSSetShaderResources(0, 1, m_textureSRV.GetAddressOf());
    m_context->PSSetSamplers(0, 1, m_samplerState.GetAddressOf());

    // 3. Draw the quad over the backbuffer
    m_context->Draw(4, 0);
}

void Graphics::BeginFrame(float r, float g, float b)
{
    float color[4] = { r, g, b, 1.0f };
    m_context->ClearRenderTargetView(m_renderTargetView.Get(), color);
    m_context->OMSetRenderTargets(1, m_renderTargetView.GetAddressOf(), nullptr);
}

void Graphics::EndFrame()
{
    m_swapChain->Present(1, 0); // V-Sync enabled
}