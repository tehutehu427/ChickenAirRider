#pragma once
#include"ItemBase.h"

class BattleItemBase : public ItemBase
{
public:

	//バトル用アイテムの種類
	enum class BATTLE_ITEM_TYPE
	{
		CANNON,		//大砲
	};

	//コンストラクタ
	BattleItemBase(const VECTOR& _pos, const VECTOR& _vec, const int _imageId, const int _modelId, const BATTLE_ITEM_TYPE _battleItemType);

	//デストラクタ
	~BattleItemBase(void)override;

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

private:

	//画像ID
	int imgId_;

	//バトルアイテム
	BATTLE_ITEM_TYPE battleItemType_;
};

