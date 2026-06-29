#include "../pch.h"
#include "../Manager/System/SceneManager.h"
#include "../Manager/System/ResourceManager.h"
#include "../Manager/System/KeyConfig.h"
#include "../Manager/System/SplitScreenManager.h"
#include "../Manager/System/Camera.h"
#include "../Manager/Game/HUDManager.h"
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
	ranks_.clear();
}

void LastGameBase::Init(void)
{
	//インスタンス
	auto& res = ResourceManager::GetInstance();
	auto& plMng = PlayerManager::GetInstance();
	auto& snd = SoundManager::GetInstance();
	auto& hud = HUDManager::GetInstance();
	auto& split = SplitScreenManager::GetInstance();

	//プレイヤーの人数
	const int plNum = GameSetting::GetInstance().GetPlayerNum();

	//スカイドーム
	sky_ = std::make_unique<SkyDome>();
	sky_->Load();
	sky_->Init();

	//HPとチャージゲージを表示
	for (int i = 0; i < GameSetting::GetInstance().GetUserNum(); i++)
	{
		hud.SetVisible(i, HUDManager::HUD_TYPE::CHARGE_GAUGE, true);
		hud.SetVisible(i, HUDManager::HUD_TYPE::HEALTH, true);
		split.SetShader(i, SplitScreenManager::SHADER_TYPE::GOD_RAY);
	}

	//順位用
	nowRank_ = plNum;

	//終了判定
	isEnd_ = false;

	//プレイヤーが機体から降りれなくする
	plMng.PlayerCanGetOff(false);

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
			ConfirmRank(pl->GetPlayerIndex());
		}
		
		//プレイヤーマネージャーに送る
		plMng.SetRanks(ranks_);

		//タイトルへ
		SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::RESULT, true, true);
	}
}

void LastGameBase::Draw(const Camera& _camera)
{
}

void LastGameBase::Release(void)
{
	//インスタンス
	auto& setting = GameSetting::GetInstance();
	auto& hud = HUDManager::GetInstance();
	auto& snd = SoundManager::GetInstance();
	auto& split = SplitScreenManager::GetInstance();

	//プレイヤーの数リセット
	setting.ResetPlayerNum();

	//HPとチャージゲージを非表示
	for (int i = 0; i < GameSetting::GetInstance().GetUserNum(); i++)
	{
		hud.SetVisible(i, HUDManager::HUD_TYPE::CHARGE_GAUGE, false);
		hud.SetVisible(i, HUDManager::HUD_TYPE::HEALTH, false);
		split.SetShader(i, SplitScreenManager::SHADER_TYPE::DEFAULT);
	}

	//BGMストップ
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

void LastGameBase::ConfirmRank(const std::unordered_map<int, int> _ranks)
{
	//順位を張り付ける
	ranks_ = _ranks;

	//順位確定
	nowRank_ = 1;
}

void LastGameBase::DebugDraw(void)
{
	DrawString(0, 0, L"LastGame", 0xffffff);
}
