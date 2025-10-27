#pragma once
#include<string>
#include "../ObjectBase.h"

class StageObject : public ObjectBase
{
public:
	//コンストラクタ
	StageObject(const int _modelId, const VECTOR _pos, const VECTOR _scl, const Quaternion _quaRot);
	
	//デストラクタ
	~StageObject(void)override;

	//読み込み
	void Load(void)override;

	//初期化
	void Init(void)override;

	//更新
	void Update(void)override;

	//描画
	void Draw(void)override;

	//当たり判定処理
	void OnHit(const std::weak_ptr<Collider> _hitCol)override;

private:

	//ステージの大きさ
	static constexpr VECTOR STAGE_SIZE = { 10000.0f,10.0f,10000.0f };

	//モデルサイズ
	static constexpr float MODEL_SIZE_Y = 200.0f;

	//色
	unsigned int color_;
};
