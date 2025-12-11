#include "../Common/Pixel/PixelShader2DHeader.hlsli"

// 定数バッファ：スロット4番目(b4と書く)
cbuffer cbParam : register(b4)
{
    float2 center;       // 中心UV
    float radiusInner;   // 内側半径UV
    float radiusOuter;   // 外側半径UV
    float progress;      // 0～1 回転ゲージ
    float3 dummy;
}

float PI = 3.14159265;

float4 main(PS_INPUT PSInput) : SV_TARGET
{
    float2 d = PSInput.uv - center;
    float dist = length(d);

    // リングの範囲外は透明
    if (dist < radiusInner || dist > radiusOuter)
        discard;
    
    // 角度 0～2PI
    float ang = atan2(d.y * progress, d.x * progress);
    //if (ang < 0) ang += 2 * PI;

    // マスク画像（白だけ有効）
    float mask = tex.Sample(texSampler, PSInput.uv).r;
    if (mask < 0.5)
        discard;

    // ゲージ進行度
    float maxAng = progress * 2 * PI;
    if (ang < maxAng)
        discard;

    // mainTex を使わず「赤色」で塗る
    return float4(1.0, 0.2, 0.2, 1.0);   // 赤ゲージ
}
