#include "../pch.h"
#include "../Application.h"
#include "../Utility/Utility.h"
#include "../Manager/System/KeyConfig.h"
#include "../Manager/System/SceneManager.h"
#include "../Manager/System/ResourceManager.h"
#include "../Manager/System/SoundManager.h"
#include "../Manager/System/SplitScreenManager.h"
#include "../Manager/Game/HUDManager.h"
#include "../Manager/Game/Timer.h"
#include "../Manager/Game/GameSetting.h"
#include "../Object/SkyDome/SkyDome.h"
#include "GameCheck.h"

GameCheck::GameCheck(SceneGame& _parent)
	: GameBase(_parent)
{
	update_[static_cast<int>(CHECK_STATE::PLAYER_PARAM)] = &GameCheck::UpdatePlayerParam;
	update_[static_cast<int>(CHECK_STATE::LAST_GAME)] = &GameCheck::UpdateLastGame;

	draw_[static_cast<int>(CHECK_STATE::PLAYER_PARAM)] = &GameCheck::DrawPlayerParam;
	draw_[static_cast<int>(CHECK_STATE::LAST_GAME)] = &GameCheck::DrawLastGame;

	state_ = CHECK_STATE::PLAYER_PARAM;
}

GameCheck::~GameCheck(void)
{
}

void GameCheck::Init(void)
{
	//インスタンス
	auto& res = ResourceManager::GetInstance();
	auto& snd = SoundManager::GetInstance();

	//タイマー設定
	timer_ = std::make_unique<Timer>();
	timer_->Init(LAST_GAME_CHECK_TIME);
	timer_->SetCountValid(true);
	timer_->SetCountView(false);

	//初期化
	state_ = CHECK_STATE::PLAYER_PARAM;

	//パラメーター確認を表示
	for (int i = 0; i < GameSetting::GetInstance().GetUserNum(); i++)
	{
		HUDManager::GetInstance().SetVisible(i, HUDManager::HUD_TYPE::PARAMETER, true);
	}

	//画像
	lastGameImage_[static_cast<int>(SceneGame::LAST_GAME_TYPE::DEATH_MATCH)] = res.Load(ResourceManager::SRC::DEATH_MATCH_CHECK_IMAGE).handleId_;
	lastGameImage_[static_cast<int>(SceneGame::LAST_GAME_TYPE::AIR_GLIDER)] = res.Load(ResourceManager::SRC::AIR_GLIDER_CHECK_IMAGE).handleId_;
	lastGameTitle_[static_cast<int>(SceneGame::LAST_GAME_TYPE::DEATH_MATCH)] = res.Load(ResourceManager::SRC::DEATH_MATCH_CHECK_TITLE).handleId_;
	lastGameTitle_[static_cast<int>(SceneGame::LAST_GAME_TYPE::AIR_GLIDER)] = res.Load(ResourceManager::SRC::AIR_GLIDER_CHECK_TITLE).handleId_;

	//BGM再生
	snd.Play(SoundManager::SOUND_NAME::SELECT_BGM, SoundManager::PLAYTYPE::LOOP);
}

void GameCheck::Update(void)
{
	//更新
	(this->*update_[static_cast<int>(state_)])();
}

void GameCheck::Draw(const Camera& _camera)
{
#ifdef _DEBUG

	//デバッグ描画
	DebugDraw();

#endif // _DEBUG

	//描画
	(this->*draw_[static_cast<int>(state_)])();
}

void GameCheck::Release(void)
{
	//BGMストップ
	auto& snd = SoundManager::GetInstance();
	snd.Stop(SoundManager::SOUND_NAME::SELECT_BGM);
}

void GameCheck::DebugDraw(void)
{
	//シーン名
	DrawString(0, 0, L"GameCheck", 0xffffff);
}

void GameCheck::UpdatePlayerParam(void)
{
	//インスタンス
	auto& key = KeyConfig::GetInstance();

	//決定
	if (key.IsTrgDown(KeyConfig::CONTROL_TYPE::ENTER, KeyConfig::JOYPAD_NO::PAD1))
	{
		//最終ゲーム確認へ
		state_ = CHECK_STATE::LAST_GAME;

		//分割画面を一つに
		auto& split = SplitScreenManager::GetInstance();
		split.CreateSplitViews(1, Application::SCREEN_SIZE_X, Application::SCREEN_SIZE_Y);

		//パラメーター確認を非表示
		for (int i = 0; i < GameSetting::GetInstance().GetUserNum(); i++)
		{
			HUDManager::GetInstance().SetVisible(i, HUDManager::HUD_TYPE::PARAMETER, false);
		}
	}
}

void GameCheck::UpdateLastGame(void)
{
	//タイマー更新
	timer_->Update();

	//時間制限を過ぎたか
	if (timer_->IsTimeOver())
	{
		//分割画面を人数分に
		auto& scnMng = SceneManager::GetInstance();
		auto& split = SplitScreenManager::GetInstance();
		const int plNum = GameSetting::GetInstance().GetUserNum();
		split.CreateSplitViews(plNum, Application::SCREEN_SIZE_X, Application::SCREEN_SIZE_Y);
		
		//カメラの設定
		for (int i = 0; i < plNum; i++)
		{
			split.SetCamera(i, scnMng.GetCamera(i));
		}

		//最終ゲームへ
		parent_.ChangeGameState(SceneGame::GAME_STATE::LAST);
	}
}

void GameCheck::DrawPlayerParam(void)
{
	//UI側で表示
}

void GameCheck::DrawLastGame(void)
{
	//最終ゲーム番号
	int lastGame = static_cast<int>(parent_.GetLastGameType());

	//最終ゲームの表示
	DrawExtendGraph(0, 0, Application::SCREEN_SIZE_X, Application::SCREEN_SIZE_Y, lastGameImage_[lastGame], true);

	//テキストボックス
	DrawBox(0, Application::SCREEN_HALF_Y - TEXT_BOX_RANGE, Application::SCREEN_SIZE_X, Application::SCREEN_HALF_Y + TEXT_BOX_RANGE, Utility::GRAY, true);

	//最終ゲームタイトルの表示
	DrawRotaGraph(Application::SCREEN_HALF_X, Application::SCREEN_HALF_Y, LAST_GAME_IMAGE_SIZE, 0.0, lastGameTitle_[lastGame], true);
}
