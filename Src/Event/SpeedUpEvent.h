#pragma once
#include "EventBase.h"

class SpeedUpEvent : public EventBase
{
public:

	//コンストラクタ
	SpeedUpEvent(void);

	//デストラクタ
	~SpeedUpEvent(void)override;

	//開始
	void Start(void)override;

	//更新
	void Update(void)override;

	//終了
	void End(void)override;

	//現在イベント
	const EVENT_TYPE& GetType(void)const override { return EVENT_TYPE::SPEED_UP; }
};

