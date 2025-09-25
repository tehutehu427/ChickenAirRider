#pragma once
#include "LogicBase.h"

class NpcLogic : public LogicBase
{
public:

	//コンストラクタ
	NpcLogic(void);

	//デストラクタ
	~NpcLogic(void)override;

	//チャージ開始
	const bool StartCharge(void)override;
};

