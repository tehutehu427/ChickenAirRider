#include "../Common/Pixel/PixelShader2DHeader.hlsli"

// 定数バッファ：スロット4番目(b4と書く)
cbuffer cbParam : register(b4)
{
    float2 center;   // 中心UV
    float progress;  // 0～1 回転ゲージ
    float cnt;       // カウンタ
    float radius;    // 半径
    float3 dummy;
}

float4 main(PS_INPUT PSInput) : SV_TARGET
{
    //円周率
    float PI = 3.14159265;
    float TWO_PI = PI * 2;

    //最初の色
    float4 startColor = { 1.0f, 0.0f, 0.0f, 1.0f };
    
    //最後の色
    float4 endColor = { 1.0f, 0.0f, 0.0f, 1.0f };

    //UV
    float2 uv = PSInput.uv;

    //ベクトル
    float2 dir = uv - center;
    
    //長さ
    float dist = length(dir);
    
    if(dist > radius)
        discard;
    
    //角度計算（-PI～PI）
    float angle = atan2(dir.y, dir.x);
    angle = fmod(angle + TWO_PI + PI * 0.5, TWO_PI);

    //0～2PI に正規化
    //if (angle < 0)
    //    angle += TWO_PI;
    //if (angle > TWO_PI)
    //    angle -= TWO_PI;

    //0～1 の UV と同じ範囲に正規化
    float ang01 = angle / TWO_PI;

    //少し速く満タンになる
    float drawProgress = saturate(progress / 0.7);

    //progress判定
    if (ang01 >= drawProgress)
        discard;

    //満タンなら光らせる
    if (drawProgress >= 1.0)
    {
        float speed = 5.0;

        // 0～1で明滅（周期はここで調整）
        float pulse = sin(cnt * TWO_PI * speed) * 0.5 + 0.5;

        // 最低輝度 + パルス
        float intensity = 1.2 + pulse * 0.8;

        //チャージ最大時の色
        float4 maxColor = { 1.0f, 0.3f, 0.3f, 1.0f };

        //点滅させる
        return maxColor * intensity;
    }
    
    //チャージ中に色を変える
    float4 returnColor = lerp(startColor, endColor, progress);

    //色を返す
    return returnColor;
}