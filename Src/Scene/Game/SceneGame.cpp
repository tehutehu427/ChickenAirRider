#include"../pch.h"
#include"../Application.h"
#include"../Common/SingletonRegistry.h"
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
#include "SceneGame.h"

SceneGame::SceneGame(void)
{
	gameState_ = GAME_STATE::MAIN;
}

SceneGame::~SceneGame(void)
{
	PlayerManager::GetInstance().Destroy();
	StageManager::GetInstance().Destroy();
	GravityManager::GetInstance().Destroy();
	CollisionManager::GetInstance().Destroy();
	SoundManager::GetInstance().StopAll();
	SingletonRegistry::GetInstance().Delete(SingletonRegistry::DESTROY_TIMING::GAME_END);
}

void SceneGame::Load(void)
{
	//メイン
	game_.emplace(GAME_STATE::MAIN, std::make_unique<GameMain>(*this));
	game_.emplace(GAME_STATE::CHECK, std::make_unique<GameCheck>(*this));
	game_.emplace(GAME_STATE::LAST, std::make_unique<LastGameBase>(*this));
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

	//ユーザー数
	int userNum = GameSetting::GetInstance().GetUserNum();

	//画面ごとのUI情報
	UIManager::GetInstance().CreateViewports(userNum, Application::SCREEN_SIZE_X, Application::SCREEN_SIZE_Y);

	//ゲームの状態変更
	ChangeGameState(GAME_STATE::MAIN);
}

void SceneGame::Update(void)
{
	//基本更新
	game_[gameState_]->Update();

	//UI状態の更新
	UIManager::GetInstance().Update();
}

void SceneGame::Draw(void)
{
	//描画
	game_[gameState_]->Draw();
}

void SceneGame::Release(void)
{
	//解放
	game_[gameState_]->Release();
}

void SceneGame::ChangeGameState(const GAME_STATE _gameState)
{
	//解放
	Release();

	//状態変更
	gameState_ = _gameState;
	
	//初期化
	game_[gameState_]->Init();
}
