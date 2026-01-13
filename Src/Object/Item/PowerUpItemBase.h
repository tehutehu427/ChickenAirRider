#pragma once
#include"ItemBase.h"
#include "../Object/Player/Parameter/Parameter.h"

class PowerUpItemBase : public ItemBase
{
public:

	//コンストラクタ
	PowerUpItemBase(const VECTOR& _pos, const VECTOR& _vec, const int _imageId, const Parameter& _param);

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
	void OnHit(std::weak_ptr<Collider> _hitCol)override;

	//パラメーターの取得
	const Parameter& GetParam(void)const { return param_; }

private:

	//取得後表示時間
	static constexpr float GOT_DISPLAY_TIME = 1.0f;

	//パラメーター
	Parameter param_;

};

