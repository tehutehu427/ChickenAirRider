#include "../pch.h"
#include "../Manager/System/KeyConfig.h"
#include "../Manager/System/SceneManager.h"
#include "../Manager/System/ResourceManager.h"
#include "../Manager/Game/UIManager.h"
#include "../Manager/Game/Timer.h"
#include "../Manager/Game/GameSetting.h"
#include "GameCheck.h"

GameCheck::GameCheck(SceneGame& _parent)
	: GameBase(_parent)
{
	update_.emplace(CHECK_STATE::NONE, [](void) {});
	update_.emplace(CHECK_STATE::PLAYER_PARAM, [this](void) {UpdatePlayerParam(); });
	update_.emplace(CHECK_STATE::LAST_GAME, [this](void) {UpdateLastGame(); });

	draw_.emplace(CHECK_STATE::NONE, [](void) {});
	draw_.emplace(CHECK_STATE::PLAYER_PARAM, [this](void) {DrawPlayerParam(); });
	draw_.emplace(CHECK_STATE::LAST_GAME, [this](void) {DrawLastGame(); });

	state_ = CHECK_STATE::NONE;
}

GameCheck::~GameCheck(void)
{
}

void GameCheck::Init(void)
{
	//タイマー設定
	timer_ = std::make_unique<Timer>();
	timer_->Init(LAST_GAME_CHECK_TIME);
	timer_->SetCountValid(true);
	timer_->SetCountView(false);

	//初期化
	state_ = CHECK_STATE::PLAYER_PARAM;

	//パラメーター確認
	for (int i = 0; i < GameSetting::GetInstance().GetUserNum(); i++)
	{
		UIManager::GetInstance().AddDraw(UIManager::DRAW_TYPE::CHECK_PARAM,i);
	}

	//インスタンス
	auto& res = ResourceManager::GetInstance();

	//画像
	lastGameImage_.emplace(static_cast<int>(SceneGame::LAST_GAME_TYPE::AIR_GLIDER), res.Load(ResourceManager::SRC::AIR_GLIDER_CHECK_IMAGE).handleId_);
	lastGameTitle_.emplace(static_cast<int>(SceneGame::LAST_GAME_TYPE::AIR_GLIDER), res.Load(ResourceManager::SRC::AIR_GLIDER_CHECK_TITLE).handleId_);
}

void GameCheck::Update(void)
{
	//更新
	update_[state_]();
}

void GameCheck::Draw(const Camera& _camera)
{
	DebugDraw();

	//描画
	draw_[state_]();
}

void GameCheck::Release(void)
{
}

void GameCheck::DebugDraw(void)
{
	DrawString(0, 0, L"GameCheck", 0xffffff);
}

void GameCheck::UpdatePlayerParam(void)
{
	//インスタンス
	auto& key = KeyConfig::GetInstance();

	//決定
	if (key.IsTrgDown(KeyConfig::CONTROL_TYPE::ENTER, KeyConfig::JOYPAD_NO::PAD1))
	{
		//分割なし
		SceneManager::GetInstance().SetIsSplitMode(false);

		//最終ゲーム確認へ
		state_ = CHECK_STATE::LAST_GAME;

		//パラメーター確認
		for (int i = 0; i < GameSetting::GetInstance().GetUserNum(); i++)
		{
			UIManager::GetInstance().SubDraw(UIManager::DRAW_TYPE::CHECK_PARAM, i);
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
		//最終ゲームへ
		parent_.ChangeGameState(SceneGame::GAME_STATE::LAST);
	}
}

void GameCheck::DrawPlayerParam(void)
{

}

void GameCheck::DrawLastGame(void)
{
}
