#pragma once
#include<string>
#include<set>
#include "StageImportData.h"
#include "../ObjectBase.h"

class StageManager;
class Geometry;

class StageObject : public ObjectBase
{
public:

	//コンストラクタ
	StageObject(const StageImportData& _data, const int _modelId);
	
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

	//当たり判定の作成
	void CreateCollider(const Collider::TAG _tag, std::unique_ptr<Geometry> _geo);

private:

	//ステージの大きさ
	static constexpr VECTOR STAGE_SIZE = { 10000.0f,10.0f,10000.0f };

	//モデルサイズ
	static constexpr float MODEL_SIZE_Y = 200.0f;
};
