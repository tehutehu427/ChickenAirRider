#include<DxLib.h>
#include"../Manager/System/SceneManager.h"
#include"../Manager/System/Camera.h"
#include"../Manager/Game/PlayerManager.h"
#include"../Object/Player/Player.h"
#include "SceneGame.h"

SceneGame::SceneGame(void)
{
}

SceneGame::~SceneGame(void)
{
}

void SceneGame::Load(void)
{
}

void SceneGame::Init(void)
{
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
	//インスタンス
	auto& plMng = PlayerManager::GetInstance();

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
