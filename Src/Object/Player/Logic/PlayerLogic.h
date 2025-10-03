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
	/// プッシュしたか
	/// </summary>
	/// <param name=""></param>
	/// <returns>プッシュの有無(true:プッシュした)</returns>
	const bool IsPush(void) override;

	/// <summary>
	/// チャージ開始
	/// </summary>
	/// <param name=""></param>
	/// <returns>チャージ開始の有無(true:チャージ開始)</returns>
	const bool StartCharge(void) override;

	/// <summary>
	/// チャージ解放
	/// </summary>
	/// <param name=""></param>
	/// <returns>チャージ解放の有無(true:チャージ解放)</returns>
	const bool DisCharge(void) override;

	/// <summary>
	/// ターンの値
	/// </summary>
	/// <param name=""></param>
	/// <returns>回転量(-値:左回転, +値:右回転)</returns>
	const float TurnValue(void) override;
};

