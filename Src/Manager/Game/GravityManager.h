#pragma once
#include <DxLib.h>
#include <memory>
#include"../Object/Common/Transform.h"
#include"../../Common/Singleton.h"

class GravityManager : public Singleton<GravityManager>
{
	//継承元のコンストラクタ等にアクセスするため
	friend class Singleton<GravityManager>;

public:

	//重力
	static constexpr float GRAVITY_POW = 3.0f;

	//初期化
	void Init(void)override;

	/// @brief 重力計算(ジャンプ行動など)
	/// @param _dir 重力方向
	/// @param _pow 重力計算に使う変数
	/// @param _per 重力倍率
	void CalcGravity(const VECTOR& _dir, VECTOR& _pow, const float _per = 1.0f);

	//重力計算
	void Calculate(void);

private:

	//コンストラクタ
	GravityManager(void);

	//デストラクタ
	~GravityManager(void)override;

	//回転用
	Transform trans_;

	//回転力
	float slerpPow_;
};

