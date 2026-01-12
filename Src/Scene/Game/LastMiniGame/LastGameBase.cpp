#include "../pch.h"
#include "../Manager/System/SceneManager.h"
#include "../Manager/System/KeyConfig.h"
#include "../Manager/Game/GameSetting.h"
#include "LastGameBase.h"

LastGameBase::LastGameBase(SceneGame& _parent)
	: GameBase(_parent)
{
}

LastGameBase::~LastGameBase(void)
{
}

void LastGameBase::Init(void)
{
}

void LastGameBase::Update(void)
{
	//インスタンス
	auto& key = KeyConfig::GetInstance();

	//決定
	if (key.IsTrgDown(KeyConfig::CONTROL_TYPE::ENTER, KeyConfig::JOYPAD_NO::PAD1))
	{
		//タイトルへ
		SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::TITLE, true, true);
	}
}

void LastGameBase::Draw(void)
{
	DebugDraw();
}

void LastGameBase::Release(void)
{
	//プレイヤーの数リセット
	GameSetting::GetInstance().ResetPlayerNum();
}

void LastGameBase::DebugDraw(void)
{
	DrawString(0, 0, L"LastGame", 0xffffff);
}
