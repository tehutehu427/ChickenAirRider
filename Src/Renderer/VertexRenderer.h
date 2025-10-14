#pragma once

class VertexMaterial;

class VertexRenderer
{
public:

	// ピクセルシェーダ用オリジナル定数バッファの使用開始スロット
	static constexpr int CONSTANT_BUF_SLOT_BEGIN_VS = 7;
	
	//コンストラクタ
	VertexRenderer(VertexMaterial& material);

	//デストラクタ
	~VertexRenderer();

	//描画処理
	void Draw(const int& modelId);

private:

	//モデルのマテリアル
	VertexMaterial& modelMaterial_;

};

