// VS/PS共通
#include "../Common/VertexToPixelHeader.hlsli"
// IN
#define PS_INPUT VertexToPixelLit
//PS
#include "../Common/Pixel/PixelShader3DHeader.hlsli"


// 定数バッファ：スロット4番目(b4と書く)
cbuffer cbParam : register(b4)
{
    float3 g_camera_pos; // カメラの位置
    float g_time;
}

float4 main(PS_INPUT PSInput) : SV_TARGET0
{
    float4 color;
    float2 uv;
    uv.x = (PSInput.uv.x + g_time) % 1.0f;
    uv.y = PSInput.uv.y;
    color = diffuseMapTexture.Sample(diffuseMapSampler, uv);
    if (color.a < 0.01f)
    {
        // 描画しない(アルファテスト)
        discard;
    }
    return color;
}