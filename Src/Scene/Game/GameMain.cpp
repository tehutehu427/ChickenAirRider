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
#include"../Manager/Game/PlayerManager.h"
#include"../Manager/Game/ItemManager.h"
#include"../Manager/Game/UIManager.h"
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
	auto& scnMng = SceneManager::GetInstance();
	auto& setMng = GameSetting::GetInstance();
	auto& grvMng = GravityManager::GetInstance();
	auto& stgMng = StageManager::GetInstance();
	auto& plMng = PlayerManager::GetInstance();
	auto& itemMng = ItemManager::GetInstance();
	auto& res = ResourceManager::GetInstance();
	auto& snd = SoundManager::GetInstance();
	auto& ui = UIManager::GetInstance();

	//タイマーの開始
	scnMng.GetTimer().SetCountValid(true);
	scnMng.GetTimer().SetCountView(true);
	scnMng.GetTimer().SetTimeLimit(setMng.GetTimeLimit());

	//重力
	grvMng.Init();

	//ステージ管理
	stgMng.Load();
	stgMng.Init(StageManager::MODE::MAIN);

	//プレイヤーの初期化
	plMng.Init();

	//スカイドーム
	sky_ = std::make_unique<SkyDome>();
	sky_->Load();
	sky_->Init();

	//プレイヤー人数
	const int plNum = GameSetting::GetInstance().GetUserNum();

	//スカイドームの設定
	for (int i = 0; i < plNum; i++)
	{
		plMng.GetPlayer(i).GetCamera().lock()->SetSkyDome(sky_);
	}

	//BGM読み込み
	int id = res.Load(ResourceManager::SRC::MAIN_GAME_BGM).handleId_;
	snd.Add(SoundManager::SOUND_NAME::MAIN_GAME_BGM, id, SoundManager::TYPE::BGM);

	//BGM再生
	snd.Play(SoundManager::SOUND_NAME::MAIN_GAME_BGM, SoundManager::PLAYTYPE::LOOP);
}

void GameMain::Update(void)
{
	//タイムリミットになったならパラメーターの確認へ
	if (SceneManager::GetInstance().GetTimer().IsTimeOver())
	{
		//シーンの削除
		parent_.ChangeGameState(SceneGame::GAME_STATE::CHECK);
		return;
	}

	//インスタンス
	auto& colMng = CollisionManager::GetInstance();
	auto& stgMng = StageManager::GetInstance();
	auto& machineMng = MachineManager::GetInstance();
	auto& plMng = PlayerManager::GetInstance();
	auto& itemMng = ItemManager::GetInstance();
	auto& key = KeyConfig::GetInstance();

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

	//ステージの描画
	stgMng.Draw();

	//機体の描画
	machineMng.Draw();

	//アイテムの描画
	itemMng.Draw();

	//プレイヤーの描画
	plMng.Draw();
}

void GameMain::Release(void)
{
	//インスタンス
	auto& scnMng = SceneManager::GetInstance();
	auto& ui = UIManager::GetInstance();
	auto& setMng = GameSetting::GetInstance();

	//UIの削除
	for (int i = 0; i < GameSetting::GetInstance().GetUserNum(); i++)
	{
		ui.SubDraw(UIManager::DRAW_TYPE::CHARGE_GAUGE,i);
		ui.SubDraw(UIManager::DRAW_TYPE::HEALTH,i);
	}

	//タイマーの開始
	scnMng.GetTimer().SetCountValid(false);
	scnMng.GetTimer().SetCountView(false);
	scnMng.GetTimer().SetTimeLimit(setMng.GetTimeLimit());
}

void GameMain::DebugDraw(void)
{
	//シーン名
	DrawString(0, 0, L"MainGame", 0xffffff);

	DrawBox(100, 100, 924, 540, 0x0000ff, true);
}
