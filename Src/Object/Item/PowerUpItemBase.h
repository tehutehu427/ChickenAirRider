#pragma once
#include"ItemBase.h"
#include"PowerUpType.h"
#include "../Object/Player/Parameter/Parameter.h"

class PowerUpItemBase : public ItemBase
{
public:

	//コンストラクタ
	PowerUpItemBase(const VECTOR& _pos, const VECTOR& _vec, const int _imageId, const Parameter& _param, const POWER_UP_TYPE& _type);

	//デストラクタ
	~PowerUpItemBase(void)override;

	//読み込み
	void Load(void)override;

	//初期化
	void Init(void)override;

	//更新
	void Update(void)override;

	//描画
	void Draw(void)override;

	//当たり判定
	void OnHit(const Collider* _hitCol)override;

	//パラメーターの取得
	const Parameter& GetParam(void)const { return param_; }

	//パワーアップの種類の取得
	const POWER_UP_TYPE& GetType(void)const { return type_; }

private:

	//取得後表示時間
	static constexpr float GOT_DISPLAY_TIME = 1.0f;

	//パラメーター
	Parameter param_;

	//パワーアップの種類
	POWER_UP_TYPE type_;

	//更新
	void UpdateAlive(void)override;
	void UpdateGot(void)override;
	void UpdateDead(void)override;
};