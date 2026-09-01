#include "../Common/Pixel/PixelShader2DHeader.hlsli"

// 定数バッファ：スロット4番目(b4と書く)
cbuffer cbParam : register(b4)
{
    float g_glitchStrength; // 0.0 ～ 0.1くらい
    float g_time;
    float g_screenWidth;
    float g_screenHeight;
}

// 疑似乱数
float Random(float x)
{
    return frac(sin(x * 12.9898) * 43758.5453);
}

float4 main(PS_INPUT PSInput) : SV_TARGET
{
// UV座標
    float2 uv = PSInput.uv;

    // --------------------------------
    // 行ごとのランダム値を作る
    // --------------------------------

    // UVのYを細かく分割する
    float lineIndex = floor(uv.y * 100.0f);

    // 時間も入れて、毎フレーム少し変化させる
    float noise = Random(lineIndex + floor(g_time * 20.0f));

    // --------------------------------
    // 一部のラインだけGlitchさせる
    // --------------------------------

    // noiseが0.85以上のラインだけGlitch
    float glitch = step(0.85f, noise);

    // -0.5 ～ 0.5の範囲
    float offset = (noise - 0.5f)
                 * g_glitchStrength
                 * glitch;

    // X方向だけ移動
    uv.x += offset;

    // --------------------------------
    // テクスチャ取得
    // --------------------------------

    float4 srcCol = tex.Sample(texSampler, uv);

    // アルファテスト
    if (srcCol.a < 0.01f)
    {
        discard;
    }

    return srcCol;
}
