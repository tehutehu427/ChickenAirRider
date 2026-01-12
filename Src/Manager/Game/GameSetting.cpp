#include "../pch.h"
#include "GameSetting.h"

void GameSetting::Init(void)
{
}

void GameSetting::ResetPlayerNum(void)
{
	userNum_ = 1;
	npcNum_ = 0;
}

GameSetting::GameSetting(void)
{
	userNum_ = 0;
	npcNum_ = 0;
}

GameSetting::~GameSetting(void)
{
}
