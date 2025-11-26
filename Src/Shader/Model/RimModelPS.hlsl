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
	float g_specular_pow;		//反射光強度

	float4 g_ambient_color;		//環境光

	float3 g_camera_pos;		//視線位置
	float dummy2;				//ダミー２
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

	//法線
	float3 norm = PSInput.normal;

	//光
	float3 lightDot = dot(PSInput.normal, -g_light_dir);

	//目線ベクトル
	float3 toEye = normalize(g_camera_pos - PSInput.worldPos);

	//視線と法線の内積
	float cDot = dot(norm, toEye);
	//絶対値
	cDot = abs(cDot);
	//反転
	cDot = 1.0f - cDot;

	// 強弱を強める
	float rimDot = pow(cDot, 2.0f); 
	//リムライトの色
	float4 rimColor = rimDot * float4(0.0f, 1.0f, 0.0f, 1.0f);
	//拡散光
	float3 diffuse = (color.rgb * g_color * lightDot);

	//色合成
	float3 rgb = diffuse + rimColor + g_ambient_color.rgb;

	//関数の戻り値がラスタライザされる
	return  float4(rgb, color.a);
}