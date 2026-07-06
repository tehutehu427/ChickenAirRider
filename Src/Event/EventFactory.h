#pragma once
#include <memory>
#include "EventType.h"

class EventBase;

class EventFactory
{
public:

	//コンストラクタ
	EventFactory(void);

	//デストラクタ
	~EventFactory(void);

	//生成
	std::unique_ptr<EventBase> CreateGameEvent(const EVENT_TYPE& _type);

private:

	//イベント生成関数ポインタ
	using CreateEventFunc = std::unique_ptr<EventBase>(EventFactory::*)(void);
	std::array<CreateEventFunc, static_cast<int>(EVENT_TYPE::MAX)> createEventFuncs_;

	//イベント生成
	std::unique_ptr<EventBase> CreateMonoEvent(void);
	std::unique_ptr<EventBase> CreateSpeedUpEvent(void);
	std::unique_ptr<EventBase> CreateNoneChargeEvent(void);
};