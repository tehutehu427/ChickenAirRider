#include "../pch.h"
#include "../Manager/System/SceneManager.h"
#include "NoChargeEvent.h"

NoChargeEvent::NoChargeEvent(void)
{
}

NoChargeEvent::~NoChargeEvent(void)
{
}

void NoChargeEvent::Start(void)
{
	//初期化
	cnt_ = 0.0f;
}

void NoChargeEvent::Update(void)
{
	//カウンタの更新
	cnt_ += SceneManager::GetInstance().GetDeltaTime();
	//終了判定
	if (cnt_ >= LIMIT_TIME)
	{
		isEnd_ = true;
	}
}

void NoChargeEvent::End(void)
{
	//初期化
	cnt_ = 0.0f;
}
