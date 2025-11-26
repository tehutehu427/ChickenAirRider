// VS/PS‹¤’Ê
#include "../Common/VertexToPixelHeader.hlsli"

// IN
#include "../Common/Vertex/VertexInputType.hlsli"
#define VERTEX_INPUT DX_MV1_VERTEX_TYPE_NMAP_1FRAME

// OUT
#define VS_OUTPUT VertexToPixelLit
#include "../Common/Vertex/VertexShader3DHeader.hlsli"

VS_OUTPUT main(VS_INPUT VSInput)
{
    VS_OUTPUT o;
    o.pos = mul(float4(input.pos, 1.0f), gWorldViewProj);
    o.uv = input.uv;
    return o;
}