#pragma once
#include "LogicBase.h"

class PlayerLogic : public LogicBase
{
public:

	//コンストラクタ
	PlayerLogic(void);

	//デストラクタ
	~PlayerLogic(void)override;

	//チャージ開始
	const bool StartCharge(void)override;

};

