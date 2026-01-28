#include "../Common/Pixel/PixelShader2DHeader.hlsli"

// 定数バッファ：スロット4番目(b4と書く)
cbuffer cbParam : register(b4)
{
    float3 color;   // 中心UV
    float dummy;
}

float4 main(PS_INPUT PSInput) : SV_TARGET
{
    //画像
    float4 imgColor = tex.Sample(texSampler, PSInput.uv);

    //合算
    float4 returnColor = imgColor * float4(color, 1.0f);
    
    //色を返す
    return returnColor;
}
