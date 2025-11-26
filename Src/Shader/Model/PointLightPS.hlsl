// VS/PS共通
#include "../Common/VertexToPixelHeader.hlsli"

// IN
#define PS_INPUT VertexToPixelLit

// PS
#include "../Common/Pixel/PixelShader3DHeader.hlsli"

// 定数バッファ：スロット7番目
cbuffer cbParam : register(b4)
{
	float3 g_camera_pos;
	float dummy1;
	float g_fog_start;
	float g_fog_end;
	float2 dummy2;
	float3 g_light_pos;
	float dummy3;
	//float g_light_num;			//←配列の要素数を受け取る
	//float4 g_light_pos[30];		//←配列で受け取る
}


float4 main(PS_INPUT PSInput) : SV_TARGET0
{
	//テクスチャーの色を取得
	float4 color; 
	color = diffuseMapTexture.Sample(diffuseMapSampler, PSInput.uv);

	//頂点とカメラの距離
	float cameraLength = length(PSInput.worldPos.xyz - g_camera_pos);
	
	//フォグの範囲
	float fogRange = g_fog_end - g_fog_start;

	//フォグ
	float fog = (g_fog_end - cameraLength) / fogRange;

	//0.0～1.0の範囲に収める
	fog = clamp(fog, 0.0f, 1.0f);

	float3 fogColor = color.rgb * fog;

	return float4(fogColor, color.a);
}