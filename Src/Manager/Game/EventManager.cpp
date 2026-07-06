#include "../../pch.h"
#include "../../Event/EventBase.h"
#include "EventManager.h"

void EventManager::Start(const EVENT_TYPE& _type)
{
	//終了処理
	End();

	//生成
	currentEvent_ = eventFactory_.CreateGameEvent(_type);

	//開始処理
	if (currentEvent_ != nullptr)
	{
		currentEvent_->Start();
	}
}

void EventManager::Update(void)
{
	//更新処理
	if (currentEvent_ != nullptr)
	{
		currentEvent_->Update();

		//終了判定
		if (currentEvent_->IsEnd())
		{
			End();
		}
	}
}

void EventManager::End(void)
{
	//現在のイベントが存在するなら終了処理
	if (currentEvent_ != nullptr)
	{
		currentEvent_->End();
		currentEvent_ = nullptr;
	}
}

EventManager::EventManager(void)
{
}

EventManager::~EventManager(void)
{
}
