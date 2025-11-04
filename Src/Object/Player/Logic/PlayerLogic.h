#pragma once
#include "LogicBase.h"
#include "../Common/Vector2F.h"

class PlayerLogic : public LogicBase
{
public:

	//コンストラクタ
	PlayerLogic(void);

	//デストラクタ
	~PlayerLogic(void)override;

	/// @brief プッシュしたか
	/// @param  
	/// @return プッシュの有無(true:プッシュした)
	const bool IsPush(void) override;

	/// @brief チャージ開始
	/// @param  
	/// @return チャージ開始の有無(true:チャージ開始)
	const bool StartCharge(void) override;

	/// @brief チャージ解放
	/// @param  
	/// @return チャージ解放の有無(true:チャージ解放)
	const bool DisCharge(void) override;

	/// @brief ターンの値
	/// @param  
	/// @return 回転量(-値:左回転, +値:右回転)
	const Vector2F TurnValue(void) override;

private:

	//回転量
	static constexpr float TURN_POW = 1.0f;

	//チャージまでのプッシュ時間
	static constexpr float CHARGE_START_PUSH_TIME = 0.1f;
};

