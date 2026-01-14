#pragma once
#include"../ItemBase.h"

class BattleItemBase : public ItemBase
{
public:

	//バトル用アイテムの種類
	enum class BATTLE_ITEM_TYPE
	{
		CANNON,		//大砲
		MAX
	};

	//コンストラクタ
	BattleItemBase(const VECTOR& _pos, const VECTOR& _vec);

	//デストラクタ
	virtual ~BattleItemBase(void)override;

	//読み込み
	virtual void Load(void)override;

	//初期化
	virtual void Init(void)override;

	//更新
	virtual void Update(void)override;

	//描画
	virtual void Draw(void)override;

	//当たり判定
	virtual void OnHit(std::weak_ptr<Collider> _hitCol)override;

protected:

	//効果時間
	static constexpr float ACTIVE_TIME = 10.0f;

	//相対座標
	static constexpr float LOCAL_POS_Y = 30.0f;

	//画像ID
	int imgId_;

	//モデル自身の大きさ
	VECTOR modelScl_;
};

