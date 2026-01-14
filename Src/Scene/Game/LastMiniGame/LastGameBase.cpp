#include "../pch.h"
#include "../Manager/System/SceneManager.h"
#include "../Manager/System/KeyConfig.h"
#include "../Manager/Game/GameSetting.h"
#include "../Manager/Game/PlayerManager.h"
#include "LastGameBase.h"

LastGameBase::LastGameBase(SceneGame& _parent)
	: GameBase(_parent)
{
	nowRank_ = 0;
}

LastGameBase::~LastGameBase(void)
{
}

void LastGameBase::Init(void)
{
	//インスタンス
	auto& plMng = PlayerManager::GetInstance();

	//プレイヤーの人数
	const int plNum = GameSetting::GetInstance().GetPlayerNum();

	//順位用
	nowRank_ = plNum;

	//プレイヤーが機体から降りれなくする
	plMng.PlayerCanGetOff(false);
}

void LastGameBase::Update(void)
{
	//インスタンス
	auto& key = KeyConfig::GetInstance();
	auto& plMng = PlayerManager::GetInstance();

	//決定
	if (key.IsTrgDown(KeyConfig::CONTROL_TYPE::ENTER, KeyConfig::JOYPAD_NO::PAD1))
	{
		//タイトルへ
		SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::TITLE, true, true);
	}

	//順位が決まった
	if (IsComfirmRanks())
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

void LastGameBase::ConfirmRank(const int _playerIndex)
{
	//順位決定
	ranks_[_playerIndex] = nowRank_;
	nowRank_--;
}

void LastGameBase::DebugDraw(void)
{
	DrawString(0, 0, L"LastGame", 0xffffff);
}
