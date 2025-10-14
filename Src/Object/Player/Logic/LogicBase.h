#pragma once

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
	virtual const float TurnValue(void) = 0;
};

