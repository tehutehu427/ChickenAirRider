#include "../Common/Pixel/PixelShader2DHeader.hlsli"

// 定数バッファ
cbuffer cbParam : register(b4)
{
    float4 g_color;   // 風の色

    float g_time;     // 経過時間
    float g_speed;    // 風の速度
    float g_strength; // 風の強さ
    float g_density;  // 風の線の密度

    float g_lineWidth;  // 線の太さ
    float g_lineLength; // 線の長さ
    float2 padding; 
};


//------------------------------------------------------------
// 簡易乱数
//------------------------------------------------------------
float Hash21(float2 p)
{
    p = frac(p * float2(123.34f, 456.21f));
    p += dot(p, p + 45.32f);

    return frac(p.x * p.y);
}


//------------------------------------------------------------
// 風の線
//------------------------------------------------------------
float WindLine(float2 uv)
{
    // Y方向に帯を作る
    float y = uv.y * g_density;

    float row = floor(y);
    float localY = frac(y);

    // 行ごとにランダム値
    float random = Hash21(float2(row, 0.0f));

    // 線を表示するかどうか
    float enable = step(0.35f, random);

    // 線のY位置
    float linePos = frac(random * 7.13f);

    float lineY = abs(localY - linePos);

    // 線の太さ
    float lineIndex =
        1.0f -
        smoothstep(
            0.0f,
            g_lineWidth,
            lineY
        );

    //--------------------------------------------------------
    // X方向へ流す
    //--------------------------------------------------------

    float x = frac(
        uv.x - g_time * g_speed + random
    );

    //--------------------------------------------------------
    // 線の長さ
    //--------------------------------------------------------

    float lengthMask =
        1.0f -
        smoothstep(
            g_lineLength,
            g_lineLength + 0.05f,
            x
        );

    return lineIndex * lengthMask * enable;
}


float4 main(PS_INPUT PSInput) : SV_TARGET
{
    //--------------------------------------------------------
    // 元画像
    //--------------------------------------------------------

    float4 srcCol =
        tex.Sample(
            texSampler,
            PSInput.uv
        );

    if (srcCol.a < 0.01f)
    {
        discard;
    }


    //--------------------------------------------------------
    // 風
    //--------------------------------------------------------

    float wind =
        WindLine(PSInput.uv);


    //--------------------------------------------------------
    // 風を加算
    //--------------------------------------------------------

    float3 windColor =
        g_color.rgb;

    srcCol.rgb +=
        windColor *
        wind *
        g_strength;


    return srcCol;
}