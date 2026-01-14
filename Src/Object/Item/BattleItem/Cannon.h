#pragma once
#include "BattleItemBase.h"

class CannonShot;

class Cannon : public BattleItemBase
{
public:

	//コンストラクタ
	Cannon(const VECTOR& _pos, const VECTOR& _vec);

	//デストラクタ
	~Cannon(void)override;

	//読み込み
	void Load(void)override;

	//初期化
	void Init(void)override;

	//更新
	void Update(void)override;

	//描画
	void Draw(void)override;

private:

	//大きさ
	static constexpr float SCALE = 0.4f;

	//生成間隔
	static constexpr float CREATE_TIME = 1.0f;

	//大砲の角度
	static constexpr float ANGLE_X = -30.0f;

	//弾
	std::vector<std::unique_ptr<CannonShot>> shots_;

	//生成カウンタ
	float createCnt_;

	//弾の生成
	void CreateShot(void);
};

