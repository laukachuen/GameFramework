// BufferShaders.hlsl

// Vertex Shader input structure
struct VS_INPUT
{
    float4 pos : POSITION; // 2D position inside the clip space
    float2 uv : TEXCOORD0; // Texture coordinates for mapping the screen buffer
};

// Pixel Shader input structure (passed from Vertex Shader)
struct PS_INPUT
{
    float4 pos : SV_POSITION; // System value for position required by pipeline
    float2 uv : TEXCOORD0; // Passed interpolation of texture coordinates
};

// Texture and Sampler resources registered to specific slots
Texture2D screenTexture : register(t0); // The dynamic texture we fill on CPU
SamplerState colorSampler : register(s0); // Filtering method for rendering

// Vertex Shader: Pass-through logic for full-screen quad
PS_INPUT VS_Main(VS_INPUT input)
{
    PS_INPUT output;
    output.pos = input.pos; // Direct mapping without transformation matrices
    output.uv = input.uv; // Forward the UV coordinates directly to pixel shader
    return output;
}

// Pixel Shader: Sampling the texture onto the screen
float4 PS_Main(PS_INPUT input) : SV_TARGET
{
    // Sample the exact pixel color from our CPU buffer texture
    return screenTexture.Sample(colorSampler, input.uv);
}