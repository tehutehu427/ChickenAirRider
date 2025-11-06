#include"../pch.h"
#include"../Manager/System/SceneManager.h"
#include"../Manager/System/Camera.h"
#include"../Manager/Game/CollisionManager.h"
#include"../Manager/Game/GravityManager.h"
#include"../Manager/Game/Timer.h"
#include"../Manager/Game/StageManager.h"
#include"../Manager/Game/CharacterManager.h"
#include"../Manager/Game/PlayerManager.h"
#include"../Object/SkyDome/SkyDome.h"
#include"../Object/Player/Player.h"
#include "SceneGame.h"

SceneGame::SceneGame(void)
{
}

SceneGame::~SceneGame(void)
{
	GravityManager::GetInstance().Destroy();
	CollisionManager::GetInstance().Destroy();
	PlayerManager::GetInstance().Destroy();
	CharacterManager::GetInstance().Destroy();
	StageManager::GetInstance().Destroy();
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

	//キャラクター情報管理の生成
	CharacterManager::CreateInstance();

	//プレイヤー管理の生成
	PlayerManager::CreateInstance();

	//インスタンス
	auto& grvMng = GravityManager::GetInstance();
	auto& stgMng = StageManager::GetInstance();
	auto& plMng = PlayerManager::GetInstance();
	auto camera = SceneManager::GetInstance().GetCamera(0).lock();

	//重力
	grvMng.Init();

	//タイマー
	timer_ = std::make_unique<Timer>();
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
	auto& plMng = PlayerManager::GetInstance();
	auto camera = SceneManager::GetInstance().GetCamera(0).lock();

	//当たり判定更新
	colMng.Update();

	//タイマーの更新
	timer_->Update();

	//スカイドームの更新
	sky_->Update();

	//ステージ更新
	stgMng.Update();

	//プレイヤーの更新
	plMng.Update();

	//カメラの回転
	//VECTOR euler = plMng.GetPlayer(0).GetTrans().quaRot.ToEuler();
	//euler.x = 0.0f;
	//camera->SetAngles(euler);

	//当たり判定の破棄
	colMng.Sweep();
}

void SceneGame::Draw(void)
{
#ifdef _DEBUG

	//デバッグ描画
	DebugDraw();

#endif // _DEBUG

	//インスタンス
	auto& stgMng = StageManager::GetInstance();
	auto& plMng = PlayerManager::GetInstance();

	//タイマーの描画
	timer_->Draw();

	//スカイドーム
	sky_->Draw();

	//ステージの描画
	stgMng.Draw();

	//プレイヤーの描画
	plMng.Draw();
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
