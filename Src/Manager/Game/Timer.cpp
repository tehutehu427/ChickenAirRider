#include "../pch.h"
#include "../Manager/System/SceneManager.h"
#include "Timer.h"

Timer::Timer(const float _timeLimit)
{
	cnt_ = 0.0f;
	timer_ = _timeLimit;
	cntValid_ = false;
}

Timer::~Timer(void)
{
}

void Timer::Init(void)
{
	cnt_ = 0.0f;
	cntValid_ = false;
}

void Timer::Update(void)
{
	//デルタタイム
	float delta = SceneManager::GetInstance().GetDeltaTime();

	//カウントが無効化されているならスキップ
	if (!cntValid_) return;

	//カウントアップ
	cnt_ += delta;
}

void Timer::Draw(void)
{
	//経過時間
	DrawFormatString(0, 64, 0xffffff, L"%02d:%02d", Minute(), Second());
}

const bool Timer::IsTimeOver(void)
{
	//時間制限あり
	if (IsTimeLimit())
	{
		//時間制限を経過時間が超えた
		return timer_ < cnt_;
	}
	//時間制限なし
	else
	{
		//問答無用でfalse
		return false;
	}
}

const float Timer::RemainingTime(void)
{
	//返す値
	float ret;

	//残り時間 (制限時間 - 経過時間)
	ret = timer_ - cnt_;
	
	//下限
	if (ret < 0.0f)ret = 0.0f;

	return ret;
}

const int Timer::Minute(void) const
{
	//分
	int ret = static_cast<int>(cnt_ / SECOND_TO_MINUTE);

	return ret;
}

const int Timer::Second(void) const
{
	//秒
	int ret = static_cast<int>(cnt_) % static_cast<int>(SECOND_TO_MINUTE);

	return ret;
}
