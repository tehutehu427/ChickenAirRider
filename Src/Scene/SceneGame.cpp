#include"../pch.h"
#include"../Application.h"
#include"../Common/SingletonRegistry.h"
#include"../Manager/System/SceneManager.h"
#include"../Manager/System/Camera.h"
#include"../Manager/Game/CollisionManager.h"
#include"../Manager/Game/GravityManager.h"
#include"../Manager/Game/Timer.h"
#include"../Manager/Game/StageManager.h"
#include"../Manager/Game/MachineManager.h"
#include"../Manager/Game/AnimationManager.h"
#include"../Manager/Game/PlayerManager.h"
#include"../Manager/Game/ItemManager.h"
#include"../Object/SkyDome/SkyDome.h"
#include"../Object/Player/Player.h"
#include "SceneGame.h"

SceneGame::SceneGame(void)
{
}

SceneGame::~SceneGame(void)
{
	PlayerManager::GetInstance().Destroy();
	StageManager::GetInstance().Destroy();
	GravityManager::GetInstance().Destroy();
	CollisionManager::GetInstance().Destroy();
	SingletonRegistry::GetInstance().Delete(SingletonRegistry::DESTROY_TIMING::GAME_END);
}

void SceneGame::Load(void)
{
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

	//インスタンス
	auto& grvMng = GravityManager::GetInstance();
	auto& stgMng = StageManager::GetInstance();
	auto& plMng = PlayerManager::GetInstance();
	auto& itemMng = ItemManager::GetInstance();
	auto camera = SceneManager::GetInstance().GetCamera(0).lock();

	//重力
	grvMng.Init();

	//タイマー
	timer_ = std::make_unique<Timer>(120);
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

	//カメラ設定
	camera->SetFollow(&plMng.GetPlayer(0).GetTrans());
	camera->ChangeMode(Camera::MODE::FOLLOW_LEAP);
}

void SceneGame::Update(void)
{
	//タイムリミットになったならリザルトへ(デバッグ)
	if (timer_->IsTimeOver())
	{
		//シーンの削除
		SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::TITLE,true, true);
		return;
	}

	//インスタンス
	auto& colMng = CollisionManager::GetInstance();
	auto& stgMng = StageManager::GetInstance();
	auto& machineMng = MachineManager::GetInstance();
	auto& plMng = PlayerManager::GetInstance();
	auto& itemMng = ItemManager::GetInstance();
	auto camera = SceneManager::GetInstance().GetCamera(0).lock();
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

	if (GetJoypadNum() < 1)
	{
		//マウス位置の初期化
		key.SetMousePos({ Application::SCREEN_HALF_X, Application::SCREEN_HALF_Y });
	}
}

void SceneGame::Draw(void)
{
#ifdef _DEBUG

	//デバッグ描画
	DebugDraw();

#endif // _DEBUG

	//インスタンス
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

void SceneGame::Release(void)
{
}

void SceneGame::DebugDraw(void)
{
	//シーン名
	DrawString(0, 0, L"SceneGame", 0xffffff);

	DrawBox(100, 100, 924, 540, 0x0000ff, true);
}
