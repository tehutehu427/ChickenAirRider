#pragma once

class LogicBase
{
public:

	//コンストラクタ
	LogicBase(void);

	//デストラクタ
	virtual ~LogicBase(void);

	//チャージ開始
	virtual const bool StartCharge(void) = 0;
};

