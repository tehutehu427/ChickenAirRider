#pragma once
#include"../Common/Vector2F.h"

class LogicBase
{
public:

	//コンストラクタ
	LogicBase(void);

	//デストラクタ
	virtual ~LogicBase(void);

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
};

