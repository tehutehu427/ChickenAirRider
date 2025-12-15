#include "../Common/Pixel/PixelShader2DHeader.hlsli"

// 定数バッファ：スロット4番目(b4と書く)
cbuffer cbParam : register(b4)
{
    float2 center;       // 中心UV
    float progress;      // 0～1 回転ゲージ
    float cnt;           // カウンタ
}

float4 main(PS_INPUT PSInput) : SV_TARGET
{
    float PI = 3.14159265;
    float TWO_PI = PI * 2;

    // 画像によるマスク処理（白だけ有効）
    float mask = tex.Sample(texSampler, PSInput.uv).r;
    if (mask < 0.5)
        discard;

    float4 startColor = { 1.0f, 1.0f, 0.2f, 1.0f };
    float4 endColor = { 0.6f, 0.6f, 1.0f, 1.0f };

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
        discard;

    //満タンなら光らせる
    if (progress >= 1.0)
    {
        float speed = 5.0;

        // 0～1で明滅（周期はここで調整）
        float pulse = sin(cnt * TWO_PI * speed) * 0.5 + 0.5;

        // 最低輝度 + パルス
        float intensity = 1.2 + pulse * 0.8;

        float4 maxColor = { 0.5f,0.5f,1.0f,1.0f };

        return maxColor * intensity;
    }
    
    float4 returnColor = lerp(startColor, endColor, progress);

    return returnColor;
}
