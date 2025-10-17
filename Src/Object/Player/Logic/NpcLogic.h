#pragma once
#include "LogicBase.h"
#include "../Common/Vector2F.h"

class NpcLogic : public LogicBase
{
public:

	//コンストラクタ
	NpcLogic(void);

	//デストラクタ
	~NpcLogic(void)override;

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
};

