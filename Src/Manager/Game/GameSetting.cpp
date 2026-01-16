#include "../pch.h"
#include "GameSetting.h"

void GameSetting::Init(void)
{
	timeLimit_ = INIT_TIME_LIMIT;
	screenSize_ = true;
}

void GameSetting::ResetPlayerNum(void)
{
	userNum_ = 1;
	npcNum_ = 0;
}

void GameSetting::SetScreenSize(const bool _screenSize)
{
	ChangeWindowMode(_screenSize);
	screenSize_ = _screenSize;
}

GameSetting::GameSetting(void)
{
	userNum_ = 0;
	npcNum_ = 0;
	timeLimit_ = 0;
	screenSize_ = false;
}

GameSetting::~GameSetting(void)
{
}
