#pragma once
#include "../../Common/Singleton.h"
#include"../../Event/EventFactory.h"
#include"../../Event/EventType.h"

class EventBase;

class EventManager : public Singleton<EventManager>
{
	//継承元のコンストラクタ等にアクセスするため
	friend class Singleton<EventManager>;

public:

	//イベント開始
	void Start(const EVENT_TYPE& _type);

	//イベント更新
	void Update(void);

	//イベント終了
	void End(void);

private:

	//イベント生成
	EventFactory eventFactory_;

	//現在のイベント
	std::unique_ptr<EventBase> currentEvent_;

	//コンストラクタ
	EventManager(void);

	//デストラクタ
	~EventManager(void)override;
};

