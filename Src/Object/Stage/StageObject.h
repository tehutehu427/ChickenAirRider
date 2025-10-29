#pragma once
#include<string>
#include<set>
#include "StageImportData.h"
#include "../ObjectBase.h"

class StageManager;

class StageObject : public ObjectBase
{
public:
	//コンストラクタ
	StageObject(const StageImportData& _data, const int _modelId, const std::set<Collider::TAG> _tags);
	
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

	//データの保有
	StageImportData data_;

	//当たり判定形状
	std::unordered_map<std::string, std::function<std::unique_ptr<Geometry>(void)>> createGeo_;

	//色
	unsigned int color_;

	//形状生成
	std::unique_ptr<Geometry> MakeSphere(void);
	std::unique_ptr<Geometry> MakeCapsule(void);
	std::unique_ptr<Geometry> MakeCube(void);
	std::unique_ptr<Geometry> MakeLine(void);
	std::unique_ptr<Geometry> MakeModel(void);
};
