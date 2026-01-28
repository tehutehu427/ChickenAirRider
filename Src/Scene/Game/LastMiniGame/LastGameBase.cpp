#include "../pch.h"
#include "../Manager/System/SceneManager.h"
#include "../Manager/System/ResourceManager.h"
#include "../Manager/System/KeyConfig.h"
#include "../Manager/System/Camera.h"
#include "../Manager/Game/GameSetting.h"
#include "../Manager/Game/PlayerManager.h"
#include "../Manager/System/SoundManager.h"
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
	auto& res = ResourceManager::GetInstance();
	auto& plMng = PlayerManager::GetInstance();
	auto& snd = SoundManager::GetInstance();

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

	//終了判定
	isEnd_ = false;

	//プレイヤーが機体から降りれなくする
	plMng.PlayerCanGetOff(false);

	//画面分割
	if (plNum > 1)
	{
		SceneManager::GetInstance().SetIsSplitMode(true);
	}

	//BGM読み込み
	int id = res.Load(ResourceManager::SRC::LAST_GAME_BGM).handleId_;
	snd.Add(SoundManager::SOUND_NAME::LAST_GAME_BGM, id, SoundManager::TYPE::BGM);

	//BGM再生
	snd.Play(SoundManager::SOUND_NAME::LAST_GAME_BGM, SoundManager::PLAYTYPE::LOOP);

}

void LastGameBase::Update(void)
{
	//インスタンス
	auto& key = KeyConfig::GetInstance();
	auto& setting = GameSetting::GetInstance();
	auto& plMng = PlayerManager::GetInstance();

	//プレイヤーの人数
	const int plNum = GameSetting::GetInstance().GetPlayerNum();

	//スカイドームの更新
	sky_->Update();

	//プレイヤーが二人以上で　かつ　順位が確定した
	if (plNum > 1 && nowRank_ < 2)
	{
		isEnd_ = true;
	}

	//順位が決まった
	if (isEnd_)
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

void LastGameBase::Draw(const Camera& _camera)
{
}

void LastGameBase::Release(void)
{
	//プレイヤーの数リセット
	GameSetting::GetInstance().ResetPlayerNum();

	//BGMストップ
	auto& snd = SoundManager::GetInstance();
	snd.Stop(SoundManager::SOUND_NAME::LAST_GAME_BGM);
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
