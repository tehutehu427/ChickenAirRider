#pragma once
#include "LogicBase.h"

class PlayerLogic : public LogicBase
{
public:

	//コンストラクタ
	PlayerLogic(void);

	//デストラクタ
	~PlayerLogic(void)override;

	/// <summary>
	/// チャージ開始
	/// </summary>
	/// <param name=""></param>
	/// <returns>チャージ開始の有無(true:チャージ開始)</returns>
	const bool StartCharge(void)const override;

	/// <summary>
	/// ターンの値
	/// </summary>
	/// <param name=""></param>
	/// <returns>回転量(-値:左回転, +値:右回転)</returns>
	const float TurnValue(void)const override;
};

