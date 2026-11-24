cbuffer ConstantBuffer : register(b0)
{
    row_major float4x4 transform;
};

struct VSInput
{
    float3 position : POSITION;
    float3 color : COLOR;
};

struct VSOutput
{
    float4 position : SV_POSITION;
    float3 color : COLOR;
};

VSOutput VS_Main(VSInput input)
{
    VSOutput output;
    
    output.position = mul(float4(input.position, 1.0f), transform);    
    output.color = input.color;

    return output;
}

float4 PS_Main(VSOutput input) : SV_TARGET
{
    return float4(input.color, 1.0f);
}