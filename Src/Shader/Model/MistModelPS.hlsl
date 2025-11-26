// VS/PS共通
#include "../Common/VertexToPixelHeader.hlsli"
// IN
#define PS_INPUT VertexToPixelLit
//PS
#include "../Common/Pixel/PixelShader3DHeader.hlsli"

SamplerState noiseSampler                 : register(s11);    		// ノイズテクスチャ
Texture2D    noiseTexture                 : register(t11);	    	// ノイズテクスチャ

// 定数バッファ：スロット4番目(b4と書く)
cbuffer cbParam : register(b4)
{
    float3 g_diffuse_color; // 基本色
    float g_time;           //カウンタ
    float g_noiselevel;
    float3 dummy;
}

float4 main(PS_INPUT PSInput) : SV_TARGET0
{
    float4 color;
    float4 noise;
    float2 uv;
    uv.x = PSInput.uv.x;
    uv.y = PSInput.uv.y + g_time * 0.1f;
    color = diffuseMapTexture.Sample(diffuseMapSampler, uv);
    noise = noiseTexture.Sample(noiseSampler, (PSInput.uv * 8.0f) % 1.0f);
    
    float threshold = (g_time * 0.2) % 1.0f;

    if (noise.r < threshold)
    {
        // 描画しない(アルファテスト)
        discard;
    }
    else if (noise.r < threshold + 0.08f && threshold > 0.01f)
    {
        color *= float4(0.5f, 0.5f, 0.5f, 1.0f);
        color.a = 1.0f;
    }

    return color * float4(g_diffuse_color, 1.0f);
}