#include "../Common/Pixel/PixelShader2DHeader.hlsli"

// 定数バッファ：スロット4番目(b4と書く)
cbuffer cbParam : register(b4)
{
    float2 center;       // 中心UV
    float progress;      // 0～1 回転ゲージ
    float dummy;
}

float4 main(PS_INPUT PSInput) : SV_TARGET
{
    float PI = 3.14159265;
    float TWO_PI = PI * 2;

    // 画像によるマスク処理（白だけ有効）
    float mask = tex.Sample(texSampler, PSInput.uv).r;
    if (mask < 0.5)
        discard;

    float4 color = {1.0, 1.0, 0.4, 1.0};
    float2 uv = PSInput.uv;
    float2 dir = uv - center;

    // === 角度計算（-PI～PI）===
    float angle = atan2(dir.y, dir.x);

    //二つの個所から
    angle *= 2;

    //右上から開始にする
    angle += PI * 0.4;

    // === 0～2PI に正規化 ===
    if (angle < 0) angle += TWO_PI;
    if (angle > TWO_PI) angle -= TWO_PI;

    // === 0～1 の UV と同じ範囲に正規化 ===
    float ang01 = angle / TWO_PI * 1.2;

    // === progress判定 ===
    if (ang01 > progress)
        return float4(0, 0, 0, 0);
    
    if (progress == 1)
        color = float4(0.7f, 0.7f, 1.0f, 1.0f);

    return color;
}
