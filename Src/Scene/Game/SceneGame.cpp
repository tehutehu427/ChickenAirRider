#include"../pch.h"
#include"../Common/SingletonRegistry.h"
#include"../Utility/Utility.h"
#include"../Manager/System/SceneManager.h"
#include"../Manager/System/SoundManager.h"
#include"../Manager/Game/GameSetting.h"
#include"../Manager/Game/HUDManager.h"
#include"../Manager/Game/CollisionManager.h"
#include"../Manager/Game/GravityManager.h"
#include"../Manager/Game/StageManager.h"
#include"../Manager/Game/MachineManager.h"
#include"../Manager/Game/AnimationManager.h"
#include"../Manager/Game/PlayerManager.h"
#include"../Manager/Game/ItemManager.h"
#include"../Manager/Game/EventManager.h"
#include"../Scene/Game/GameMain.h"
#include"../Scene/Game/GameCheck.h"
#include"../Scene/Game/LastMiniGame/DeathMatch.h"
#include"../Scene/Game/LastMiniGame/AirGlider.h"
#include "SceneGame.h"

SceneGame::SceneGame(void)
{
	gameState_ = GAME_STATE::MAIN;

	//メイン
	createGame_[static_cast<int>(GAME_STATE::MAIN)] = &SceneGame::CreateGameMain;
	createGame_[static_cast<int>(GAME_STATE::CHECK)] = &SceneGame::CreateGameCheck;

	//最終ゲーム
	createLastGame_[static_cast<int>(LAST_GAME_TYPE::DEATH_MATCH)] = &SceneGame::CreateLastGameDeathMatch;
	createLastGame_[static_cast<int>(LAST_GAME_TYPE::AIR_GLIDER)] = &SceneGame::CreateLastGameAirGlider;
	
	//ソロ用最終ゲーム判定
	soloLastGameJudge_[static_cast<int>(LAST_GAME_TYPE::DEATH_MATCH)] = false;
	soloLastGameJudge_[static_cast<int>(LAST_GAME_TYPE::AIR_GLIDER)] = true;
}

SceneGame::~SceneGame(void)
{
	//最終ゲームの解放
	game_->Release();

	//HUDの初期化
	auto& hud = HUDManager::GetInstance();
	hud.Init();

	//サウンドの停止
	SoundManager::GetInstance().StopAll();
	
	//ゲーム関連のインスタンスの削除
	SingletonRegistry::GetInstance().Delete(SingletonRegistry::DESTROY_TIMING::GAME_END);
}

void SceneGame::Load(void)
{
	ResetLastGame();
}

void SceneGame::Init(void)
{
	//インスタンス削除タイミング
	auto timing = SingletonRegistry::DESTROY_TIMING::GAME_END;

	//HUDの初期化
	HUDManager::GetInstance().Init();

	//当たり判定管理の生成
	CollisionManager::CreateInstance(timing);

	//重力制御
	GravityManager::CreateInstance(timing);

	//ステージ管理の生成
	StageManager::CreateInstance(timing);

	//機体管理の生成
	MachineManager::CreateInstance(timing);

	//キャラクター情報管理の生成
	AnimationManager::CreateInstance(timing);

	//プレイヤー管理の生成
	PlayerManager::CreateInstance(timing);

	//アイテム管理の生成
	ItemManager::CreateInstance(timing);

	//イベント管理の生成
	EventManager::CreateInstance(timing);

	//ゲームの状態変更
	ChangeGameState(GAME_STATE::MAIN);
}

void SceneGame::Update(void)
{
	//基本更新
	game_->Update();

	//HUD更新
	HUDManager::GetInstance().Update();
}

void SceneGame::Draw(const Camera& _camera)
{
	//描画
	game_->Draw(_camera);
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
	game_ = std::move((this->*createGame_[static_cast<int>(_gameState)])());
	
	//初期化
	game_->Init();
}

void SceneGame::ResetLastGame(void)
{
	//プレイヤーの数
	const int plNum = GameSetting::GetInstance().GetPlayerNum();

	//ランダム
	LAST_GAME_TYPE rand = static_cast<LAST_GAME_TYPE>(Utility::GetRandomValue(0, static_cast<int>(LAST_GAME_TYPE::MAX) - 1));

	//ソロなら
	if (plNum <= 1)
	{
		//判定
		bool judge = soloLastGameJudge_[static_cast<int>(rand)];

		//ソロでできるゲームにする
		while (!judge)
		{
			rand = static_cast<LAST_GAME_TYPE>(Utility::GetRandomValue(0, static_cast<int>(LAST_GAME_TYPE::MAX) - 1));
			judge = soloLastGameJudge_[static_cast<int>(rand)];
		}
	}

	//保存
	lastGameType_ = rand;

	//最終ゲーム生成
	createGame_[static_cast<int>(GAME_STATE::LAST)] = createLastGame_[static_cast<int>(lastGameType_)];
}

std::unique_ptr<GameBase> SceneGame::CreateGameMain(void)
{
	return std::make_unique<GameMain>(*this);
}

std::unique_ptr<GameBase> SceneGame::CreateGameCheck(void)
{
	return std::make_unique<GameCheck>(*this);
}

std::unique_ptr<GameBase> SceneGame::CreateLastGameDeathMatch(void)
{
	return std::make_unique<DeathMatch>(*this);
}

std::unique_ptr<GameBase> SceneGame::CreateLastGameAirGlider(void)
{
	return std::make_unique<AirGlider>(*this);	
}
