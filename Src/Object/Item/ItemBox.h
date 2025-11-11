#pragma once
#include "../ObjectBase.h"

class ItemBox : public ObjectBase
{
public:

	//コンストラクタ
	ItemBox(VECTOR _pos);

	//デストラクタ
	~ItemBox(void)override;

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

private:

	//当たり判定の半分
	static constexpr VECTOR BOX_HALF = { 100.0f,100.0f,100.0f };

	//死亡判定
	bool isDead_;
};