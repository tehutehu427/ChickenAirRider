// VS/PS共通
#include "../Common/VertexToPixelHeader.hlsli"

// 入力
#include "../Common/Vertex/VertexInputType.hlsli"
#define VERTEX_INPUT DX_MV1_VERTEX_TYPE_NMAP_8FRAME

// 出力
#define VS_OUTPUT VertexToPixelLit
#include "../Common/Vertex/VertexShader3DHeader.hlsli"

#define L_W_MAT g_localWorldMatrix.lwMatrix

VS_OUTPUT main(VS_INPUT input)
{
    VS_OUTPUT output;

    //----------------------------------
    // スキニング行列作成
    //----------------------------------

    float4 skinMat[3] =
    {
        float4(0, 0, 0, 0),
        float4(0, 0, 0, 0),
        float4(0, 0, 0, 0)
    };

    int4 idx;
    float4 weight;

    // 1～4ボーン
    idx = input.blendIndices0;
    weight = input.blendWeight0;

    skinMat[0] += L_W_MAT[idx.x + 0] * weight.x;
    skinMat[1] += L_W_MAT[idx.x + 1] * weight.x;
    skinMat[2] += L_W_MAT[idx.x + 2] * weight.x;

    skinMat[0] += L_W_MAT[idx.y + 0] * weight.y;
    skinMat[1] += L_W_MAT[idx.y + 1] * weight.y;
    skinMat[2] += L_W_MAT[idx.y + 2] * weight.y;

    skinMat[0] += L_W_MAT[idx.z + 0] * weight.z;
    skinMat[1] += L_W_MAT[idx.z + 1] * weight.z;
    skinMat[2] += L_W_MAT[idx.z + 2] * weight.z;

    skinMat[0] += L_W_MAT[idx.w + 0] * weight.w;
    skinMat[1] += L_W_MAT[idx.w + 1] * weight.w;
    skinMat[2] += L_W_MAT[idx.w + 2] * weight.w;

    // 5～8ボーン
    idx = input.blendIndices1;
    weight = input.blendWeight1;

    skinMat[0] += L_W_MAT[idx.x + 0] * weight.x;
    skinMat[1] += L_W_MAT[idx.x + 1] * weight.x;
    skinMat[2] += L_W_MAT[idx.x + 2] * weight.x;

    skinMat[0] += L_W_MAT[idx.y + 0] * weight.y;
    skinMat[1] += L_W_MAT[idx.y + 1] * weight.y;
    skinMat[2] += L_W_MAT[idx.y + 2] * weight.y;

    skinMat[0] += L_W_MAT[idx.z + 0] * weight.z;
    skinMat[1] += L_W_MAT[idx.z + 1] * weight.z;
    skinMat[2] += L_W_MAT[idx.z + 2] * weight.z;

    skinMat[0] += L_W_MAT[idx.w + 0] * weight.w;
    skinMat[1] += L_W_MAT[idx.w + 1] * weight.w;
    skinMat[2] += L_W_MAT[idx.w + 2] * weight.w;

    //----------------------------------
    // 頂点
    //----------------------------------

    float4 localPos = float4(input.pos, 1);

    float4 worldPos;

    worldPos.x = dot(localPos, skinMat[0]);
    worldPos.y = dot(localPos, skinMat[1]);
    worldPos.z = dot(localPos, skinMat[2]);
    worldPos.w = 1.0f;

    output.worldPos = worldPos.xyz;

    //----------------------------------
    // 法線
    //----------------------------------

    float4 localNormal = float4(input.norm, 0);

    output.normal.x = dot(localNormal, skinMat[0]);
    output.normal.y = dot(localNormal, skinMat[1]);
    output.normal.z = dot(localNormal, skinMat[2]);
    output.normal = normalize(output.normal);

    //----------------------------------
    // View
    //----------------------------------

    float4 viewPos;

    viewPos.xyz = mul(worldPos, g_base.viewMatrix);
    viewPos.w = 1;

    output.vwPos = viewPos;

    //----------------------------------
    // Projection
    //----------------------------------

    output.svPos = mul(viewPos, g_base.projectionMatrix);

    //----------------------------------
    // その他
    //----------------------------------

    output.uv = input.uv0.xy;

    output.diffuse = input.diffuse;

    output.lightDir = float3(0, 0, 0);
    output.lightAtPos = float3(0, 0, 0);

    return output;
}