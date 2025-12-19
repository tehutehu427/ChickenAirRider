#include"../pch.h"
#include"../Application.h"
#include"../Common/SingletonRegistry.h"
#include"../Manager/System/SceneManager.h"
#include"../Manager/System/ResourceManager.h"
#include"../Manager/System/SoundManager.h"
#include"../Manager/System/Camera.h"
#include"../Manager/Game/CollisionManager.h"
#include"../Manager/Game/GameSetting.h"
#include"../Manager/Game/GravityManager.h"
#include"../Manager/Game/Timer.h"
#include"../Manager/Game/StageManager.h"
#include"../Manager/Game/MachineManager.h"
#include"../Manager/Game/AnimationManager.h"
#include"../Manager/Game/PlayerManager.h"
#include"../Manager/Game/ItemManager.h"
#include"../Scene/Game/GameMain.h"
#include "SceneGame.h"

SceneGame::SceneGame(void)
{
	gameState_ = GAME_STATE::NONE;
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
	//サウンド
	auto& snd = SoundManager::GetInstance();
	auto& res = ResourceManager::GetInstance();

	//追加
	int id = res.Load(ResourceManager::SRC::MAIN_GAME_BGM).handleId_;
	snd.Add(SoundManager::SOUND_NAME::MAIN_GAME_BGM, id, SoundManager::TYPE::BGM);

	//メイン
	game_.emplace(GAME_STATE::MAIN, std::make_unique<GameMain>(*this));
	//game_.emplace(GAME_STATE::CHECK, std::make_unique<GameMain>());
	//game_.emplace(GAME_STATE::LAST, std::make_unique<GameMain>());
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

	//ゲームの状態
	gameState_ = GAME_STATE::MAIN;

	//初期化
	game_[gameState_]->Init();
}

void SceneGame::Update(void)
{
	game_[gameState_]->Update();
}

void SceneGame::Draw(void)
{
	game_[gameState_]->Draw();
}

void SceneGame::Release(void)
{
	game_[gameState_]->Release();
}