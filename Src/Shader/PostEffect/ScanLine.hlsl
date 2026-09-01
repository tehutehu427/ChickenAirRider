#include "../Common/Pixel/PixelShader2DHeader.hlsli"

// 定数バッファ：スロット4番目(b4と書く)
cbuffer cbParam : register(b4)
{
	float  g_time;
    float3 dummy;
}

float4 main(PS_INPUT PSInput) : SV_TARGET
{
    float2 uv = PSInput.uv;

    // 元画像
    float4 srcCol = tex.Sample(texSampler, uv);

    if (srcCol.a < 0.01f)
    {
        discard;
    }

    // ============================================================
    // 走査線
    // ============================================================

    float scanLine = sin(uv.y * 100.0f - g_time * 5.0f);

    // -1～1 → 0～1
    scanLine = scanLine * 0.5f + 0.5f;

    // 元画像をほとんど暗くしない
    srcCol.rgb += scanLine * 0.02f;


    // ============================================================
    // スキャン位置
    // ============================================================

    // 上から下まで移動する時間
    float moveTime = 5.0f;

    // 下端に到達してから待つ時間
    float waitTime = 0.5f;

    // 1ループにかかる時間
    float cycleTime = moveTime + waitTime;

    // 現在のループ内時間
    float currentTime = fmod(g_time, cycleTime);

    float scanPos = 0.0f;
    float scan = 0.0f;

    // ============================================================
    // スキャン移動中
    // ============================================================

    if (currentTime < moveTime)
    {
        // 0.0 → 1.0
        scanPos = currentTime / moveTime;

        // スキャン位置との距離
        float distance = abs(uv.y - scanPos);

        // スキャン本体の幅
        float scanWidth = 0.015f;

        // スキャン本体
        scan = 1.0f -
               smoothstep(0.0f, scanWidth, distance);

        // --------------------------------------------------------
        // 下端に近づいたらフェードアウト
        // --------------------------------------------------------

        float fadeStart = 0.9f;

        float fade = 1.0f -
                     smoothstep(fadeStart, 1.0f, scanPos);

        scan *= fade;
    }
    else
    {
        // ========================================================
        // 下端で待機中
        // ========================================================

        // スキャンは完全に消す
        scan = 0.0f;
    }


    // ============================================================
    // スキャン本体を明るくする
    // ============================================================

    srcCol.rgb += scan * 0.25f;


    // ============================================================
    // スキャン周辺の光
    // ============================================================

    if (currentTime < moveTime)
    {
        float distance = abs(uv.y - scanPos);

        // 本体より広い範囲
        float glowWidth = 0.08f;

        float glow = 1.0f -
                     smoothstep(0.0f, glowWidth, distance);

        // 下端に近づいたらこちらもフェード
        float fadeStart = 0.9f;

        float fade = 1.0f -
                     smoothstep(fadeStart, 1.0f, scanPos);

        glow *= fade;

        srcCol.rgb += glow * 0.05f;
    }


    return srcCol;
}
