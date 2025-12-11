#include "../Common/Pixel/PixelShader2DHeader.hlsli"

cbuffer cbColor : register(b4)
{
}

float4 main(PS_INPUT PSInput) : SV_TARGET
{
	float4 Color = tex.Sample(texSampler, PSInput.uv);

	Color.r = 1 - Color.r;
	Color.g = 1 - Color.g;
	Color.b = 1 - Color.b;
	return Color;
}