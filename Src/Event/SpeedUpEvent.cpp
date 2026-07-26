#include "../pch.h"
#include "../Manager/System/SceneManager.h"
#include "SpeedUpEvent.h"

SpeedUpEvent::SpeedUpEvent(void)
{
}

SpeedUpEvent::~SpeedUpEvent(void)
{
}

void SpeedUpEvent::Start(void)
{
	//初期化
	cnt_ = 0.0f;
}

void SpeedUpEvent::Update(void)
{
	//カウンタの更新
	cnt_ += SceneManager::GetInstance().GetDeltaTime();
	//終了判定
	if (cnt_ >= LIMIT_TIME)
	{
		isEnd_ = true;
	}
}

void SpeedUpEvent::End(void)
{
	//初期化
	cnt_ = 0.0f;
}
