#include"../pch.h"
#include"../Manager/System/SceneManager.h"
#include"../Manager/System/Camera.h"
#include"../Manager/Game/Timer.h"
#include"../Manager/Game/PlayerManager.h"
#include"../Object/Player/Player.h"
#include "SceneGame.h"

SceneGame::SceneGame(void)
{
}

SceneGame::~SceneGame(void)
{
	PlayerManager::GetInstance().Destroy();
}

void SceneGame::Load(void)
{
}

void SceneGame::Init(void)
{
	//タイマー
	timer_ = std::make_unique<Timer>(10.0f);
	timer_->Init();

	//カウント開始
	timer_->SetCountValid(true);

	//プレイヤー管理の生成
	PlayerManager::CreateInstance();

	//インスタンス
	auto& plMng = PlayerManager::GetInstance();

	//プレイヤーの初期化
	plMng.Init();

	//カメラ設定
	SceneManager::GetInstance().GetCamera(0).lock()->SetFollow(&plMng.GetPlayer(0).GetTrans());
	SceneManager::GetInstance().GetCamera(0).lock()->ChangeMode(Camera::MODE::FOLLOW);
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
	auto& plMng = PlayerManager::GetInstance();

	//タイマーの更新
	timer_->Update();

	//プレイヤーの更新
	plMng.Update();

	//カメラの回転
	SceneManager::GetInstance().GetCamera(0).lock()->SetAngles(plMng.GetPlayer(0).GetTrans().quaRot.ToEuler());
}

void SceneGame::Draw(void)
{
#ifdef _DEBUG

	//デバッグ描画
	DebugDraw();

#endif // _DEBUG
	//タイマーの描画
	timer_->Draw();

	//プレイヤーの描画
	PlayerManager::GetInstance().Draw();
}

void SceneGame::Release(void)
{
}

void SceneGame::DebugDraw(void)
{
	//シーン名
	DrawString(0, 0, L"SceneGame", 0xffffff);

	DrawBox(100, 100, 924, 540, 0xffffff, true);
}
