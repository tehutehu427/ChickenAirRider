#include "../pch.h"
#include "../Manager/System/SceneManager.h"
#include "../Manager/System/KeyConfig.h"
#include "../Manager/System/Camera.h"
#include "../Manager/Game/GameSetting.h"
#include "../Manager/Game/PlayerManager.h"
#include "../Object/Player/Player.h"
#include "../Object/SkyDome/SkyDome.h"
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

	//スカイドーム
	sky_ = std::make_unique<SkyDome>();
	sky_->Load();
	sky_->Init();

	//スカイドームの設定
	for (int i = 0; i < plNum; i++)
	{
		plMng.GetPlayer(i).GetCamera().lock()->SetSkyDome(sky_);
	}

	//順位用
	nowRank_ = plNum;

	//プレイヤーが機体から降りれなくする
	plMng.PlayerCanGetOff(false);

	//画面分割
	if (plNum > 1)
	{
		SceneManager::GetInstance().SetIsSplitMode(true);
	}
}

void LastGameBase::Update(void)
{
	//インスタンス
	auto& key = KeyConfig::GetInstance();
	auto& setting = GameSetting::GetInstance();
	auto& plMng = PlayerManager::GetInstance();

	//決定
	//if (key.IsTrgDown(KeyConfig::CONTROL_TYPE::ENTER, KeyConfig::JOYPAD_NO::PAD1))
	//{
	//	//タイトルへ
	//	SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::TITLE, true, true);
	//}

	//スカイドームの更新
	sky_->Update();

	//順位が決まった
	if (IsComfirmRanks())
	{
		//プレイヤー人数
		const int plNum = setting.GetPlayerNum();

		//最後が勝者
		for (int i = 0; i < plNum; i++)
		{
			//プレイヤー
			const auto& pl = plMng.GetPlayer(i);
			ConfirmRank(pl.GetPlayerIndex());
		}

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
	//既にあるならしない
	if (ranks_.contains(_playerIndex))return;

	//順位決定
	ranks_.emplace(_playerIndex, nowRank_);
	nowRank_--;
}

void LastGameBase::DebugDraw(void)
{
	DrawString(0, 0, L"LastGame", 0xffffff);
}
