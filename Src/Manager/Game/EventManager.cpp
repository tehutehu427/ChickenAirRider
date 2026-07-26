#include "../../pch.h"
#include "../Utility/Utility.h"
#include "../System/SceneManager.h"
#include "../../Event/EventType.h"
#include "../../Event/EventBase.h"
#include "EventManager.h"

void EventManager::Start(void)
{
	//終了処理
	End();

	//ランダムでイベント
	EVENT_TYPE rand = static_cast<EVENT_TYPE>(Utility::GetRandomValue(0, static_cast<int>(EVENT_TYPE::MAX) - 1));
	
	//デバッグ(モノトーン)
	rand = EVENT_TYPE::MONO;

	//生成
	currentEvent_ = eventFactory_.CreateGameEvent(rand);

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
	//イベント外の場合
	else
	{
		//次のイベントまでカウンタ更新
		auto& scnMng = SceneManager::GetInstance();
		cnt_ += scnMng.GetDeltaTime();

		//イベント開始
		if (cnt_ >= EVENT_INTERVAL)
		{
			//初期化
			cnt_ = 0.0f;

			//イベント開始
			Start();
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
	:cnt_(0.0f)
{
}

EventManager::~EventManager(void)
{
}
