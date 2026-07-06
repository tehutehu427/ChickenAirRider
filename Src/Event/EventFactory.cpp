#include "../pch.h"
#include "MonoEvent.h"
#include "SpeedUpEvent.h"
#include "NoChargeEvent.h"
#include "EventFactory.h"

EventFactory::EventFactory(void)
{
	createEventFuncs_[static_cast<int>(EventManager::EVENT_TYPE::MONO)] = &EventFactory::CreateMonoEvent;
	createEventFuncs_[static_cast<int>(EventManager::EVENT_TYPE::SPEED_UP)] = &EventFactory::CreateSpeedUpEvent;
	createEventFuncs_[static_cast<int>(EventManager::EVENT_TYPE::NONE_CHARGE)] = &EventFactory::CreateNoneChargeEvent;
}

EventFactory::~EventFactory(void)
{
}

std::unique_ptr<EventBase> EventFactory::CreateGameEvent(const EventManager::EVENT_TYPE& _type)
{
	return (this->*createEventFuncs_[static_cast<int>(_type)])();
}

std::unique_ptr<EventBase> EventFactory::CreateMonoEvent(void)
{
	return std::make_unique<MonoEvent>();
}

std::unique_ptr<EventBase> EventFactory::CreateSpeedUpEvent(void)
{
	return std::make_unique<SpeedUpEvent>();
}

std::unique_ptr<EventBase> EventFactory::CreateNoneChargeEvent(void)
{
	return std::make_unique<NoChargeEvent>();
}
