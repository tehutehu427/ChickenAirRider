#pragma once
#include "../ObjectBase.h"
#include "../Object/Player/Parameter/Parameter.h"

class ItemBase : public ObjectBase
{
public:

	//コンストラクタ
	ItemBase(const VECTOR _pos, const Parameter& _param);

	//デストラクタ
	~ItemBase(void)override;

	//読み込み
	void Load(void)override;

	//初期化
	void Init(void)override;

	//更新
	void Update(void)override;

	//描画
	void Draw(void)override;

	//当たり判定
	void OnHit(std::weak_ptr<Collider> _hitCol)override;

	//死亡判定の取得
	const bool IsDead(void)const { return isDead_; }

	//パラメーターの取得
	const Parameter& GetParam(void)const { return param_; }

private:

	//画像の大きさ
	static constexpr float IMG_SIZE = 64.0f * 3.0f;

	//半径
	static constexpr float RADIUS = 100.0f;

	//死亡判定
	bool isDead_;

	//パラメーター
	Parameter param_;
};

