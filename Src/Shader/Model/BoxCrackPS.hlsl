// VS/PS共通
#include "../Common/VertexToPixelHeader.hlsli"

// IN
#define PS_INPUT VertexToPixelLit

// PS
#include "../Common/Pixel/PixelShader3DHeader.hlsli"

SamplerState crackSampler                 : register(s11);    		// ひびテクスチャ
Texture2D    crackTexture                 : register(t11);	    	// ひびテクスチャ

// 定数バッファ：スロット4番目(b4と書く)
cbuffer cbParam : register(b4)
{
    float crackProgress;   // 0.0 → 1.0 の進行度
    float3 dummy;
}

float4 main(PS_INPUT PSInput) : SV_TARGET0
{
    float2 uv = PSInput.uv;

    float4 baseColor = diffuseMapTexture.Sample(diffuseMapSampler, uv);
    float crackValue = crackTexture.Sample(crackSampler, uv).r;

    // crackValue は 0=白, 1=黒と仮定
    // 黒い部分が閾値以下になったらひび割れとして描画
    float threshold = lerp(1.0, 0.0, crackProgress);  // 時間で減る

    // ひび部分を抽出（黒いところ）
    float crackMask = step(crackValue, threshold);

    // ひび色（黒）
    float4 crackColor = float4(0, 0, 0, 1);

    // ひびを上に合成
    float4 result = lerp(baseColor, crackColor, crackMask);

    return result;
}