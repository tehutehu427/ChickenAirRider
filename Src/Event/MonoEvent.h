#pragma once
#include "EventBase.h"

class MonoEvent : public EventBase
{
public:

	//コンストラクタ
	MonoEvent(void);

	//デストラクタ
	~MonoEvent(void)override;

	//開始
	void Start(void)override;

	//更新
	void Update(void)override;

	//終了
	void End(void)override;

	//現在イベント
	const EVENT_TYPE& GetType(void)const override { return EVENT_TYPE::MONO; }
};

