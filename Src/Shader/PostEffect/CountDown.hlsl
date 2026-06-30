#include "../Common/Pixel/PixelShader2DHeader.hlsli"

// 定数バッファ：スロット4番目(b4と書く)
cbuffer cbParam : register(b4)
{
    float progress;     // 進行度
    float3 dummy;       
    float2 g_uvScale;   //uvの大きさ
    float2 g_uvOffset;  //uvのオフセット
}

float4 main(PS_INPUT PSInput) : SV_TARGET
{
    //=========================
    // ブラー
    //=========================

    float blur = (1.0f - progress) * 0.008f;

    float2 uv = PSInput.uv;

    uv *= g_uvScale;
    uv += g_uvOffset;

    float4 srcCol = tex.Sample(texSampler, uv);
    
    srcCol += tex.Sample(texSampler, uv);
    srcCol += tex.Sample(texSampler, uv + float2(blur, 0));
    srcCol += tex.Sample(texSampler, uv + float2(-blur, 0));
    srcCol += tex.Sample(texSampler, uv + float2(0, blur));
    srcCol += tex.Sample(texSampler, uv + float2(0, -blur));
    
    srcCol /= 5.0f;

    if (srcCol.a < 0.01f)
    {
        discard;
    }

    float4 dstCol = srcCol;

    //=========================
    // モノクロ
    //=========================

    float gray = (dstCol.r + dstCol.g + dstCol.b) / 3.0f;
    dstCol.rgb = gray.xxx;

    //=========================
    // 最初だけ少し白く
    //=========================

    float glow = 1.0f - progress;
    dstCol.rgb += glow * 0.3f;

    //=========================
    // アルファ
    //=========================

    dstCol.a *= lerp(0.2f, 1.0f, progress);

    return dstCol;
}