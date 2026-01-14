#include "../pch.h"
#include "../Manager/Game/PlayerManager.h"
#include "DeathMatch.h"

DeathMatch::DeathMatch(SceneGame& _parent)
	: LastGameBase(_parent)
{
}

DeathMatch::~DeathMatch(void)
{
}

void DeathMatch::Init(void)
{
	LastGameBase::Init();
}

void DeathMatch::Update(void)
{
	LastGameBase::Update();
}

void DeathMatch::Draw(void)
{
}

void DeathMatch::Release(void)
{
}
