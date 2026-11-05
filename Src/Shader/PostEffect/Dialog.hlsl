#include "../Common/Pixel/PixelShader2DHeader.hlsli"

cbuffer cbDialog : register(b4)
{
    float2 g_dstSize; // 描画先の幅・高さ
    float2 g_srcSize; // 元画像の幅・高さ

    float4 g_dstBorder; // 描画先の固定領域
                            // x:左 y:上 z:右 w:下

    float4 g_srcBorder; // 元画像の固定領域
                            // x:左 y:上 z:右 w:下

    float g_alpha; // 透明度
    float3 g_tintColor; // 色調整
};

// 固定領域が描画先に収まるように調整
float2 GetSafeDstBorder(float2 dstSize, float2 borderStart, float2 borderEnd)
{
    float2 total = borderStart + borderEnd;

    // 固定領域の合計が描画先を超える場合は縮小
    float2 scale = min(
        float2(1.0f, 1.0f),
        dstSize / max(total, float2(0.0001f, 0.0001f))
    );

    return scale;
}

// 1方向のUV変換
float MapNineSlice(
    float pos,
    float dstSize,
    float srcSize,
    float dstStart,
    float dstEnd,
    float srcStart,
    float srcEnd)
{
    // 描画先の固定領域が重なる場合に備える
    float total = dstStart + dstEnd;

    if (total > dstSize)
    {
        float scale = dstSize / total;
        dstStart *= scale;
        dstEnd *= scale;
    }

    // 左側・上側
    if (pos < dstStart)
    {
        if (dstStart <= 0.0001f)
            return 0.0f;

        return pos * srcStart / dstStart;
    }

    // 右側・下側
    if (pos > dstSize - dstEnd)
    {
        if (dstEnd <= 0.0001f)
            return srcSize;

        return srcSize - srcEnd
            + (pos - (dstSize - dstEnd))
            * srcEnd / dstEnd;
    }

    // 中央部分
    float dstCenter = dstSize - dstStart - dstEnd;
    float srcCenter = srcSize - srcStart - srcEnd;

    if (dstCenter <= 0.0001f)
    {
        return srcStart;
    }

    return srcStart
        + (pos - dstStart) * srcCenter / dstCenter;
}

// 9スライス用UV変換
float2 GetNineSliceUV(float2 uv)
{
    // UVを描画先のピクセル座標に変換
    float2 pos = uv * g_dstSize;

    // 描画先の固定領域
    float2 safeScale = GetSafeDstBorder(
        g_dstSize,
        g_dstBorder.xy,
        g_dstBorder.zw
    );

    float2 dstStart = g_dstBorder.xy * safeScale;
    float2 dstEnd = g_dstBorder.zw * safeScale;

    // X方向
    float u = MapNineSlice(
        pos.x,
        g_dstSize.x,
        g_srcSize.x,
        dstStart.x,
        dstEnd.x,
        g_srcBorder.x,
        g_srcBorder.z
    );

    // Y方向
    float v = MapNineSlice(
        pos.y,
        g_dstSize.y,
        g_srcSize.y,
        dstStart.y,
        dstEnd.y,
        g_srcBorder.y,
        g_srcBorder.w
    );

    // ピクセル座標からUVに戻す
    return float2(u, v) / g_srcSize;
}

float4 main(PS_INPUT PSInput) : SV_TARGET
{
    // 9スライス用UV
    float2 uv = GetNineSliceUV(PSInput.uv);

    // テクスチャ取得
    float4 color = tex.Sample(texSampler, uv);

    // 頂点カラー
    color *= PSInput.diffuse;

    // 色調整
    color.rgb *= g_tintColor;

    // 透明度
    color.a *= g_alpha;

    return color;
}