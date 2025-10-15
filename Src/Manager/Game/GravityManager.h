#pragma once
#include <DxLib.h>
#include <memory>
#include"../Object/Common/Transform.h"
class GravityManager
{
public:

	//重力
	static constexpr float GRAVITY_POW = 3.0f;

	//コンストラクタ
	GravityManager(void);
	
	//デストラクタ
	~GravityManager(void);	

	//静的にインスタンス生成
	static void CreateInstance(void);

	//静的にインスタンスを取得する
	static GravityManager& GetInstance(void);

	//解放
	static void Destroy(void);

	//初期化
	void Init(void);

	/// @brief 重力計算(ジャンプ行動など)
	/// @param _dir 重力方向
	/// @param _pow 重力計算に使う変数
	/// @param _per 重力倍率
	void CalcGravity(const VECTOR& _dir, VECTOR& _pow, const float _per = 100.0f);

	//重力計算
	void Calculate(void);

private:

	//インスタンス
	static GravityManager* instance_;

	//回転用
	Transform trans_;

	//回転力
	float slerpPow_;
};

