#pragma once

class LogicBase
{
public:

	//コンストラクタ
	LogicBase(void);

	//デストラクタ
	virtual ~LogicBase(void);

	/// <summary>
	/// プッシュしたか
	/// </summary>
	/// <param name=""></param>
	/// <returns>プッシュの有無(true:プッシュした)</returns>
	virtual const bool IsPush(void) = 0;

	/// <summary>
	/// チャージ開始
	/// </summary>
	/// <param name=""></param>
	/// <returns>チャージ開始の有無(true:チャージ開始)</returns>
	virtual const bool StartCharge(void) = 0;

	/// <summary>
	/// チャージ解放
	/// </summary>
	/// <param name=""></param>
	/// <returns>チャージ解放の有無(true:チャージ解放)</returns>
	virtual const bool DisCharge(void) = 0;

	/// <summary>
	/// ターンの値
	/// </summary>
	/// <param name=""></param>
	/// <returns>回転量(-値:左回転, +値:右回転)</returns>
	virtual const float TurnValue(void) = 0;
};

