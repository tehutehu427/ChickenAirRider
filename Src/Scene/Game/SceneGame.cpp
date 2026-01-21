#include"../pch.h"
#include"../Application.h"
#include"../Common/SingletonRegistry.h"
#include"../Utility/Utility.h"
#include"../Manager/System/SceneManager.h"
#include"../Manager/System/SoundManager.h"
#include"../Manager/Game/GameSetting.h"
#include"../Manager/Game/CollisionManager.h"
#include"../Manager/Game/UIManager.h"
#include"../Manager/Game/GravityManager.h"
#include"../Manager/Game/StageManager.h"
#include"../Manager/Game/MachineManager.h"
#include"../Manager/Game/AnimationManager.h"
#include"../Manager/Game/PlayerManager.h"
#include"../Manager/Game/ItemManager.h"
#include"../Scene/Game/GameMain.h"
#include"../Scene/Game/GameCheck.h"
#include"../Scene/Game/LastMiniGame/LastGameBase.h"
#include"../Scene/Game/LastMiniGame/DeathMatch.h"
#include"../Scene/Game/LastMiniGame/AirGlider.h"
#include "SceneGame.h"

SceneGame::SceneGame(void)
{
	gameState_ = GAME_STATE::MAIN;

	createLastGame_.emplace(LAST_GAME_TYPE::DEATH_MATCH, [this](void) {return std::make_unique<DeathMatch>(*this); });
	createLastGame_.emplace(LAST_GAME_TYPE::AIR_GLIDER, [this](void) {return std::make_unique<AirGlider>(*this); });
}

SceneGame::~SceneGame(void)
{
	PlayerManager::GetInstance().Destroy();
	StageManager::GetInstance().Destroy();
	GravityManager::GetInstance().Destroy();
	CollisionManager::GetInstance().Destroy();
	SoundManager::GetInstance().StopAll();
	SingletonRegistry::GetInstance().Delete(SingletonRegistry::DESTROY_TIMING::GAME_END);

	for (int i = 0; i < GameSetting::GetInstance().GetUserNum(); i++)
	{
		UIManager::GetInstance().SubDraw(UIManager::DRAW_TYPE::CHARGE_GAUGE,i);
		UIManager::GetInstance().SubDraw(UIManager::DRAW_TYPE::HEALTH,i);
		UIManager::GetInstance().SubDraw(UIManager::DRAW_TYPE::CHECK_PARAM,i);
	}
}

void SceneGame::Load(void)
{
	//メイン
	createGame_.emplace(GAME_STATE::MAIN, [this](void) {return std::make_unique<GameMain>(*this); });
	createGame_.emplace(GAME_STATE::CHECK, [this](void) {return std::make_unique<GameCheck>(*this); });
	ResetLastGame();
}

void SceneGame::Init(void)
{
	//当たり判定管理の生成
	CollisionManager::CreateInstance();

	//重力制御
	GravityManager::CreateInstance();

	//ステージ管理の生成
	StageManager::CreateInstance();

	//機体管理の生成
	MachineManager::CreateInstance(SingletonRegistry::DESTROY_TIMING::GAME_END);

	//キャラクター情報管理の生成
	AnimationManager::CreateInstance(SingletonRegistry::DESTROY_TIMING::GAME_END);

	//プレイヤー管理の生成
	PlayerManager::CreateInstance();

	//アイテム管理の生成
	ItemManager::CreateInstance(SingletonRegistry::DESTROY_TIMING::GAME_END);

	////ユーザー数
	//int userNum = GameSetting::GetInstance().GetUserNum();

	////画面ごとのUI情報
	//UIManager::GetInstance().CreateViewports(userNum, Application::SCREEN_SIZE_X, Application::SCREEN_SIZE_Y);

	//ゲームの状態変更
	ChangeGameState(GAME_STATE::MAIN);
}

void SceneGame::Update(void)
{
	//基本更新
	game_->Update();

	//UI状態の更新
	UIManager::GetInstance().Update();
}

void SceneGame::Draw(void)
{
	//描画
	game_->Draw();
}

void SceneGame::Release(void)
{
	if (game_ == nullptr)return;

	//解放
	game_->Release();
}

void SceneGame::ChangeGameState(const GAME_STATE _gameState)
{
	//解放
	Release();

	//状態変更
	game_ = std::move(createGame_[_gameState]());
	
	//初期化
	game_->Init();
}

void SceneGame::ResetLastGame(void)
{
	//ランダム
	LAST_GAME_TYPE rand = static_cast<LAST_GAME_TYPE>(Utility::GetRandomValue(0, static_cast<int>(LAST_GAME_TYPE::MAX) - 1));
	rand = LAST_GAME_TYPE::AIR_GLIDER;

	//最終ゲーム生成
	createGame_.emplace(GAME_STATE::LAST, [this, rand](void) {return std::move(createLastGame_[rand]()); });
}
