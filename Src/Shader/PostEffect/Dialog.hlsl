#include "../Common/Pixel/PixelShader2DHeader.hlsli"

cbuffer cbColor : register(b4)
{
    float2 g_borderUV;   // 元画像に対する角の割合
    float2 g_padding;    // 予備
    float g_alpha;       // 透明度
    float3 g_tintColor;  // 色調整
};

// 9スライス用UV変換
float2 GetNineSliceUV(float2 uv)
{
    float2 result;

    // X方向
    if (uv.x < g_borderUV.x)
    {
        result.x = uv.x * 0.5f / g_borderUV.x;
    }
    else if (uv.x > 1.0f - g_borderUV.x)
    {
        result.x = 0.5f +
            (uv.x - (1.0f - g_borderUV.x))
            * 0.5f / g_borderUV.x;
    }
    else
    {
        result.x = 0.5f;
    }

    // Y方向
    if (uv.y < g_borderUV.y)
    {
        result.y = uv.y * 0.5f / g_borderUV.y;
    }
    else if (uv.y > 1.0f - g_borderUV.y)
    {
        result.y = 0.5f +
            (uv.y - (1.0f - g_borderUV.y))
            * 0.5f / g_borderUV.y;
    }
    else
    {
        result.y = 0.5f;
    }

    return result;
}

float4 main(PS_INPUT PSInput) : SV_TARGET
{
    return float4(1.0f, 0.0f, 0.0f, 1.0f);
    
    float2 uv = GetNineSliceUV(PSInput.uv);

    float4 color = tex.Sample(texSampler, uv);

    color *= PSInput.diffuse;
    color.rgb *= g_tintColor;
    color.a *= g_alpha;

    return color;
}