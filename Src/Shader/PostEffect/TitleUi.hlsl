#include "../Common/Pixel/PixelShader2DHeader.hlsli"

// 定数バッファ：スロット4番目(b4と書く)
cbuffer cbParam : register(b4)
{
    float cnt;           // カウンタ
    float3 dummy;
}

float4 main(PS_INPUT PSInput) : SV_TARGET
{
    //色
    float4 color = tex.Sample(texSampler, PSInput.uv);

    //元々透明なところは無視
    if (color.a < 0.5)discard;

    //点滅速度
    float speed = 3.0f;

    //アルファ値
    color.a *= 0.5f + 0.5f * sin(cnt * 6.28318f * speed);

    //返す
    return color;
}