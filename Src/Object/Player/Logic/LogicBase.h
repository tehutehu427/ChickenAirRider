#pragma once
#include"../Common/Vector2F.h"

class LogicBase
{
public:

	//コンストラクタ
	LogicBase(void);

	//デストラクタ
	virtual ~LogicBase(void);

	//初期化
	virtual void Init(void) = 0;
	
	//更新
	virtual void Update(void) = 0;

#pragma region 機体

	//プッシュしたか
	virtual const bool IsPush(void)const = 0;

	//チャージ開始したか
	virtual const bool StartCharge(void)const = 0;

	//チャージ解放したか
	virtual const bool DisCharge(void)const = 0;

	//ターンの値(-値:左回転, +値:右回転)
	virtual const Vector2F TurnValue(void)const = 0;

	//スペシャルボタンを押したか
	virtual const bool IsSpecial(void)const = 0;

	//機体から降りたか
	virtual const bool IsGetOff(void)const = 0;

	//レバガチャ判定
	virtual const bool IsButtonMeshing(void) = 0;

#pragma endregion 機体

#pragma region キャラクター

	//歩きの値
	virtual const Vector2F WalkValue(void)const = 0;

	//ジャンプしたか
	virtual const bool IsJump(void)const = 0;

#pragma endregion キャラクター
};

