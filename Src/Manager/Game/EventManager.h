#pragma once
#include<memory>
#include "../../Common/Singleton.h"
#include"../../Event/EventFactory.h"
#include"../../Event/EventType.h"

class EventBase;

class EventManager : public Singleton<EventManager>
{
	//継承元のコンストラクタ等にアクセスするため
	friend class Singleton<EventManager>;

public:

	//イベント更新
	void Update(void);

	//イベント終了
	void End(void);

private:

	//イベント間隔時間
	static constexpr float EVENT_INTERVAL = 30.0f;

	//イベント生成
	EventFactory eventFactory_;

	//現在のイベント
	std::unique_ptr<EventBase> currentEvent_;

	//イベントの間隔カウンタ
	float cnt_;

	//コンストラクタ
	EventManager(void);

	//デストラクタ
	~EventManager(void)override;

	//イベント開始
	void Start(void);
};