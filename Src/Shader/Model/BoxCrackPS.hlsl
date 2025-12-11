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
    float crackVal = crackTexture.Sample(crackSampler, uv).r;

    // crackProgress = 0 → 全部ひび表示
    // crackProgress = 1 → 全くひび無し
    float threshold = crackProgress;

    float crackMask = step(threshold, crackVal);

    float4 crackColor = float4(0,0,0,1);

    return lerp(baseColor, crackColor, crackMask); 
}