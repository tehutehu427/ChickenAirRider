// VS/PS共通
#include "../Common/VertexToPixelHeader.hlsli"

// IN
#define PS_INPUT VertexToPixelLit

// PS
#include "../Common/Pixel/PixelShader3DHeader.hlsli"

// 定数バッファ：スロット4番目(b4と書く)
cbuffer cbParam : register(b4)
{
	float4 g_color;				//基本色
	float3 g_light_dir;			//光の方向
	float dummy;				//ダミー
	float4 g_ambient_color;		//環境光
}

float4 main(PS_INPUT PSInput) : SV_TARGET0
{
	float4 color;

	//テクスチャーの色を取得
	color = diffuseMapTexture.Sample(diffuseMapSampler, PSInput.uv);
	if (color.a < 0.01f)
	{
		discard;
	}

	//光
	float3 light = dot(-g_light_dir, PSInput.normal);

	float3 rgb = color.rgb * light;

	//関数の戻り値がラスタライザされる
	//return color * g_color * float4(light,1.0f) + g_ambient_color;
	return float4(rgb, color.a) + g_ambient_color;
}