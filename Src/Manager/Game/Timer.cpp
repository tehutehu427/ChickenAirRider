#include "../pch.h"
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
}

void Timer::Update(void)
{
}

void Timer::Draw(void)
{
}
