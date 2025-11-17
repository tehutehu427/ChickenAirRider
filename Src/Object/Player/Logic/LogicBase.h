#pragma once
#include"../Common/Vector2F.h"

class LogicBase
{
public:

	//コンストラクタ
	LogicBase(void);

	//デストラクタ
	virtual ~LogicBase(void);

#pragma region 機体

	/// @brief プッシュしたか
	/// @param  
	/// @return プッシュの有無(true:プッシュした)
	virtual const bool IsPush(void) = 0;

	/// @brief チャージ開始
	/// @param  
	/// @return チャージ開始の有無(true:チャージ開始)
	virtual const bool StartCharge(void) = 0;

	/// @brief チャージ解放
	/// @param  
	/// @return チャージ解放の有無(true:チャージ解放)
	virtual const bool DisCharge(void) = 0;

	/// @brief ターンの値
	/// @param  
	/// @return 回転量(-値:左回転, +値:右回転)
	virtual const Vector2F TurnValue(void) = 0;

	/// @brief スペシャルボタンを押した
	/// @param  
	/// @return スペシャルの有無(true:スペシャルを押した)
	virtual const bool IsSpecial(void) = 0;

	/// @brief 機体から降りたか
	/// @param  
	/// @return 機体からの降下の有無(true:降りた)
	virtual const bool IsGetOff(void) = 0;

	/// @brief レバガチャ判定
	/// @param  
	/// @return レバガチャ(true:レバガチャした)
	virtual const bool IsButtonMeshing(void) = 0;

#pragma endregion 機体

#pragma region キャラクター

	/// @brief 歩く
	/// @param  
	/// @return 歩きの値
	virtual const Vector2F WalkValue(void) = 0;

	/// @brief ジャンプの有無
	/// @param  
	/// @return true:ジャンプした
	virtual const bool IsJump(void) = 0;

#pragma endregion キャラクター
};

