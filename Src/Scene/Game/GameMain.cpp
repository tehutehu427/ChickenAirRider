#include "../pch.h"
#include "../Application.h"
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
#include"../Object/SkyDome/SkyDome.h"
#include"../Object/Player/Player.h"
#include "GameMain.h"

GameMain::GameMain(SceneGame& _parent)
	: GameBase(_parent)
{
}

GameMain::~GameMain(void)
{
}

void GameMain::Init(void)
{
	//インスタンス
	auto& setMng = GameSetting::GetInstance();
	auto& grvMng = GravityManager::GetInstance();
	auto& stgMng = StageManager::GetInstance();
	auto& plMng = PlayerManager::GetInstance();
	auto& itemMng = ItemManager::GetInstance();
	auto& setting = GameSetting::GetInstance();

	//重力
	grvMng.Init();

	//タイマー
	timer_ = std::make_unique<Timer>(setMng.GetTimeLimit());
	timer_->Init();

	//カウント開始
	timer_->SetCountValid(true);

	//ステージ管理
	stgMng.Load();
	stgMng.Init();

	//プレイヤーの初期化
	plMng.Init();

	//スカイドーム
	sky_ = std::make_unique<SkyDome>(plMng.GetPlayer(0).GetTrans().pos);
	sky_->Load();
	sky_->Init();

	//インスタンス
	auto& snd = SoundManager::GetInstance();

	//BGM再生
	snd.Play(SoundManager::SOUND_NAME::MAIN_GAME_BGM, SoundManager::PLAYTYPE::LOOP);
}

void GameMain::Update(void)
{
	//タイムリミットになったならリザルトへ(デバッグ)
	if (timer_->IsTimeOver())
	{
		//シーンの削除
		parent_.SetGameState(SceneGame::GAME_STATE::CHECK);
		SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::TITLE, true, true);
		return;
	}

	//インスタンス
	auto& colMng = CollisionManager::GetInstance();
	auto& stgMng = StageManager::GetInstance();
	auto& machineMng = MachineManager::GetInstance();
	auto& plMng = PlayerManager::GetInstance();
	auto& itemMng = ItemManager::GetInstance();
	auto& key = KeyConfig::GetInstance();

	//タイマーの更新
	timer_->Update();

	//スカイドームの更新
	sky_->Update();

	//ステージ更新
	stgMng.Update();

	//機体更新
	machineMng.Update();

	//プレイヤーの更新
	plMng.Update();

	//アイテムの更新
	itemMng.Update();

	//当たり判定更新
	colMng.Update();

	//当たり判定の破棄
	colMng.Sweep();

	//ゲームパッドがないならマウス操作なので
	if (GetJoypadNum() < 1)
	{
		//マウス位置の初期化
		key.SetMousePos({ Application::SCREEN_HALF_X, Application::SCREEN_HALF_Y });
	}
}

void GameMain::Draw(void)
{
#ifdef _DEBUG

	//デバッグ描画
	DebugDraw();

#endif // _DEBUG

	//インスタンス
	auto& setting = GameSetting::GetInstance();
	auto& stgMng = StageManager::GetInstance();
	auto& machineMng = MachineManager::GetInstance();
	auto& plMng = PlayerManager::GetInstance();
	auto& itemMng = ItemManager::GetInstance();

	//スカイドーム
	sky_->Draw();

	//ステージの描画
	stgMng.Draw();

	//機体の描画
	machineMng.Draw();

	//アイテムの描画
	itemMng.Draw();

	//プレイヤーの描画
	plMng.Draw();

	//タイマーの描画
	timer_->Draw();
}

void GameMain::Release(void)
{
}

void GameMain::DebugDraw(void)
{
	//シーン名
	DrawString(0, 0, L"MainGame", 0xffffff);

	DrawBox(100, 100, 924, 540, 0x0000ff, true);
}
