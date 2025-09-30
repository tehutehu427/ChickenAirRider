#pragma once

class LogicBase
{
public:

	//コンストラクタ
	LogicBase(void);

	//デストラクタ
	virtual ~LogicBase(void);

	/// <summary>
	/// チャージ開始
	/// </summary>
	/// <param name=""></param>
	/// <returns>チャージ開始の有無(true:チャージ開始)</returns>
	virtual const bool StartCharge(void)const = 0;

	/// <summary>
	/// ターンの値
	/// </summary>
	/// <param name=""></param>
	/// <returns>回転量(-値:左回転, +値:右回転)</returns>
	virtual const float TurnValue(void)const = 0;
};

