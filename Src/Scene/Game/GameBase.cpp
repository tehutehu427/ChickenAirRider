#include "../pch.h"
#include "../Object/SkyDome/SkyDome.h"
#include "GameBase.h"

GameBase::GameBase(SceneGame& _parent)
	: parent_(_parent)
{
}

GameBase::~GameBase(void)
{
}

void GameBase::DebugDraw(void)
{
}
