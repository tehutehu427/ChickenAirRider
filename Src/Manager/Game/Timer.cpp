#include "../pch.h"
#include "../Application.h"
#include "../Utility/Utility.h"
#include "../Manager/System/SceneManager.h"
#include "../Manager/System/ResourceManager.h"
#include "Timer.h"

Timer::Timer(void)
{
	cnt_ = 0.0f;
	timer_ = 0.0f;
	cntValid_ = false;
	isView_ = false;
	pos_ = {};
	colonImg_ = -1;
	frameImg_ = -1;
	numImgs_ = nullptr;
}

Timer::~Timer(void)
{
	numImgs_ = nullptr;
}

void Timer::Init(const float _timeLimit)
{
	//初期化
	cnt_ = 0.0f;
	cntValid_ = false;
	isView_ = false;
	timer_ = _timeLimit;

	//画像
	frameImg_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::TIMER_FRAME).handleId_;
	colonImg_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::COLON).handleId_;
	numImgs_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::NUMBER).handleIds_;
}

void Timer::Update(void)
{
	//デルタタイム
	float delta = SceneManager::GetInstance().GetDeltaTime();

	//カウントが無効化されているならスキップ
	if (!cntValid_) return;

	//カウントが終わったらスキップ
	if (IsTimeOver())return;

	//カウントアップ
	cnt_ += delta;
}

void Timer::Draw(void)
{
	//表示の有無
	if (!isView_)return;

	int minute = 0;
	int second = 0;

	//時間制限あり
	if (IsTimeLimit())
	{
		minute = static_cast<int>(Minute(RemainingTime()));
		second = static_cast<int>(Second(RemainingTime()));
	}
	//時間制限なし
	else
	{
		minute = static_cast<int>(Minute(cnt_));
		second = static_cast<int>(Second(cnt_));
	}

	//描画
	DrawRotaGraph(pos_.x, pos_.y, FRAME_EXRATE, 0.0, frameImg_, true);
	DrawRotaGraph(pos_.x, pos_.y, IMAGE_EXRATE, 0.0, colonImg_, true);
	DrawRotaGraph(pos_.x - IMAGE_WIDTH * 2, pos_.y, IMAGE_EXRATE, 0.0, numImgs_[minute / 10], true);
	DrawRotaGraph(pos_.x - IMAGE_WIDTH, pos_.y, IMAGE_EXRATE, 0.0, numImgs_[minute % 10], true);
	DrawRotaGraph(pos_.x + IMAGE_WIDTH, pos_.y, IMAGE_EXRATE, 0.0, numImgs_[second / 10], true);
	DrawRotaGraph(pos_.x + IMAGE_WIDTH * 2, pos_.y, IMAGE_EXRATE, 0.0, numImgs_[second % 10], true);
}

const bool Timer::IsTimeOver(void)const
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

const float Timer::RemainingTime(void)const
{
	//返す値
	float ret;

	//残り時間 (制限時間 - 経過時間)
	ret = timer_ - cnt_;
	
	//下限
	if (ret < 0.0f)ret = 0.0f;

	return ret;
}

const int Timer::Minute(const float _value) const
{
	//分
	int ret = static_cast<int>(_value / SECOND_TO_MINUTE);

	return ret;
}

const int Timer::Second(const float _value) const
{
	//秒
	int ret = static_cast<int>(_value) % static_cast<int>(SECOND_TO_MINUTE);

	return ret;
}
