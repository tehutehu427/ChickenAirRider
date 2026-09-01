#include "../pch.h"
#include "../Application.h"
#include"../Manager/System/SceneManager.h"
#include"../Manager/System/ResourceManager.h"
#include"../Manager/System/SplitScreenManager.h"
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
#include"../Manager/Game/GlobalUIManager.h"
#include"../Manager/Game/HUDManager.h"
#include"../Manager/Game/EventManager.h"
#include"../Object/SkyDome/SkyDome.h"
#include "GameMain.h"

GameMain::GameMain(SceneGame& _parent)
	: GameBase(_parent)
{
	//更新
	update_.emplace(STATE::START, [this](void) {UpdateStart(); });
	update_.emplace(STATE::GAME, [this](void) {UpdateGame(); });
	update_.emplace(STATE::FIN, [this](void) {UpdateFinish(); });

	//描画
	draw_.emplace(STATE::START, [this](const Camera& _camera) {DrawStart(_camera); });
	draw_.emplace(STATE::GAME, [this](const Camera& _camera) {DrawGame(_camera); });
	draw_.emplace(STATE::FIN, [this](const Camera& _camera) {DrawFinish(_camera); });
}

GameMain::~GameMain(void)
{
}

void GameMain::Init(void)
{
	//インスタンス
	auto& split = SplitScreenManager::GetInstance();
	auto& grvMng = GravityManager::GetInstance();
	auto& stgMng = StageManager::GetInstance();
	auto& plMng = PlayerManager::GetInstance();
	auto& itemMng = ItemManager::GetInstance();
	auto& res = ResourceManager::GetInstance();
	auto& snd = SoundManager::GetInstance();
	auto& gloUi = GlobalUIManager::GetInstance();
	auto& hud = HUDManager::GetInstance();

	//タイマーの開始
	gloUi.GetTimer().Init(COUNT_DOWN);
	gloUi.GetTimer().SetCountValid(true);
	gloUi.GetTimer().SetCountView(false);
	gloUi.SetVisible(GlobalUIManager::DRAW_TYPE::COUNT_DOWN, true);
	gloUi.SetVisible(GlobalUIManager::DRAW_TYPE::TIMER, true);

	//重力
	grvMng.Init();

	//ステージ管理
	stgMng.Init(StageManager::MODE::MAIN);

	//プレイヤーの初期化
	plMng.Init();

	//アイテムの初期化
	itemMng.Init();

	//スカイドーム
	sky_ = std::make_unique<SkyDome>();
	sky_->Load();
	sky_->Init();

	//プレイヤー人数
	const int plNum = GameSetting::GetInstance().GetUserNum();

	//HPとチャージゲージを表示
	for (int i = 0; i < plNum; i++)
	{
		hud.SetVisible(i, HUDManager::HUD_TYPE::CHARGE_GAUGE, true);
		hud.SetVisible(i, HUDManager::HUD_TYPE::HEALTH, true);
		hud.SetVisible(i, HUDManager::HUD_TYPE::PUSH_BUTTON, true);
		split.SetShader(i, SplitScreenManager::SHADER_TYPE::GOD_RAY);
	}

	//BGM読み込み
	int id = res.Load(ResourceManager::SRC::MAIN_GAME_BGM).handleId_;
	snd.Add(SoundManager::SOUND_NAME::MAIN_GAME_BGM, id, SoundManager::TYPE::BGM);

	//SE読み込み
	id = res.Load(ResourceManager::SRC::COUNT_DOWN_SE).handleId_;
	snd.Add(SoundManager::SOUND_NAME::COUNT_DOWN_SE, id, SoundManager::TYPE::SE);
	id = res.Load(ResourceManager::SRC::TIME_UP_SE).handleId_;
	snd.Add(SoundManager::SOUND_NAME::TIME_UP_SE, id, SoundManager::TYPE::SE);

	//BGM再生
	snd.Play(SoundManager::SOUND_NAME::MAIN_GAME_BGM, SoundManager::PLAYTYPE::LOOP);

	//変数の初期化
	state_ = STATE::START;
}

void GameMain::Update(void)
{
	//各更新
	update_[state_]();
}

void GameMain::Draw(const Camera& _camera)
{
	//各描画
	draw_[state_](_camera);
}

void GameMain::Release(void)
{
	//インスタンス
	auto& split = SplitScreenManager::GetInstance();
	auto& scnMng = SceneManager::GetInstance();
	auto& setMng = GameSetting::GetInstance();
	auto& gloUi = GlobalUIManager::GetInstance();
	auto& snd = SoundManager::GetInstance();
	auto& hud = HUDManager::GetInstance();

	//タイマーの削除
	gloUi.GetTimer().SetCountValid(false);
	gloUi.GetTimer().SetCountView(false);
	gloUi.GetTimer().SetTimeLimit(setMng.GetTimeLimit());
	gloUi.SetVisible(GlobalUIManager::DRAW_TYPE::TIMER, false);

	//プレイヤー人数
	const int plNum = GameSetting::GetInstance().GetUserNum();

	//HPとチャージゲージを非表示
	for (int i = 0; i < plNum; i++)
	{
		hud.SetVisible(i, HUDManager::HUD_TYPE::CHARGE_GAUGE, false);
		hud.SetVisible(i, HUDManager::HUD_TYPE::HEALTH, false);
		hud.SetVisible(i, HUDManager::HUD_TYPE::GET_OFF, false);
		split.SetShader(i, SplitScreenManager::SHADER_TYPE::DEFAULT);
	}

	//フィニッシュUIの削除
	gloUi.SetVisible(GlobalUIManager::DRAW_TYPE::FINISH, false);

	//BGMストップ
	snd.Stop(SoundManager::SOUND_NAME::MAIN_GAME_BGM);
}

void GameMain::DebugDraw(void)
{
}

void GameMain::UpdateStart(void)
{
	//インスタンス
	auto& setting = GameSetting::GetInstance();
	auto& gloUi = GlobalUIManager::GetInstance();
	auto& hud = HUDManager::GetInstance();
	auto& snd = SoundManager::GetInstance();
	
	//タイムリミットになったならゲーム開始
	if (gloUi.GetTimer().IsTimeOver())
	{
		//プレイヤー人数
		const int plNum = setting.GetUserNum();

		//タイマーのリセット
		gloUi.GetTimer().Init(setting.GetTimeLimit());
		gloUi.GetTimer().SetCountValid(true);
		gloUi.GetTimer().SetCountView(true);

		//カウントダウンの削除
		gloUi.SetVisible(GlobalUIManager::DRAW_TYPE::COUNT_DOWN, false);

		for (int i = 0; i < plNum; i++)
		{
			//入力表示を非表示
			hud.SetVisible(i, HUDManager::HUD_TYPE::PUSH_BUTTON, false);
		}

		//ゲーム開始
		state_ = STATE::GAME;
	}

	//カウントダウンSEの再生
	if (gloUi.GetTimer().IsChanged())
	{
		snd.Play(SoundManager::SOUND_NAME::COUNT_DOWN_SE, SoundManager::PLAYTYPE::BACK);
	}
}

void GameMain::UpdateGame(void)
{
	//インスタンス
	auto& scnMng = SceneManager::GetInstance();
	auto& gloUi = GlobalUIManager::GetInstance();
	auto& snd = SoundManager::GetInstance();
	auto& event = EventManager::GetInstance();

	//タイムリミットになったなら終了表示
	if (gloUi.GetTimer().IsTimeOver())
	{
		//状態遷移
		state_ = STATE::FIN;

		//イベントのストップ
		event.End();

		//カウントダウンの削除
		gloUi.SetVisible(GlobalUIManager::DRAW_TYPE::COUNT_DOWN, false);

		//タイムアップSEの再生
		snd.Play(SoundManager::SOUND_NAME::TIME_UP_SE, SoundManager::PLAYTYPE::BACK);

		//タイムアップUIの描画
		gloUi.SetVisible(GlobalUIManager::DRAW_TYPE::FINISH, true);

		return;
	}
	else if(gloUi.GetTimer().IsUnderSeconds(COUNT_DOWN))
	{
		//カウントダウンSEの再生
		if (gloUi.GetTimer().IsChanged())
		{
			snd.Play(SoundManager::SOUND_NAME::COUNT_DOWN_SE, SoundManager::PLAYTYPE::BACK);
		}

		//カウントダウンの描画
		gloUi.SetVisible(GlobalUIManager::DRAW_TYPE::COUNT_DOWN, true);
	}

	//インスタンス
	auto& colMng = CollisionManager::GetInstance();
	auto& stgMng = StageManager::GetInstance();
	auto& machineMng = MachineManager::GetInstance();
	auto& plMng = PlayerManager::GetInstance();
	auto& itemMng = ItemManager::GetInstance();
	auto& key = KeyConfig::GetInstance();

	//イベントの更新
	event.Update();

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

void GameMain::UpdateFinish(void)
{
	//カウント
	const float delta = SceneManager::GetInstance().GetDeltaTime();
	cnt_ += delta;

	//終了確認が出来たらパラメーターの確認へ
	if (cnt_ > FINISH_TIME)
	{
		//シーンの削除
		parent_.ChangeGameState(SceneGame::GAME_STATE::CHECK);
		return;
	}
}

void GameMain::DrawStart(const Camera& _camera)
{
	DrawGame(_camera);
}

void GameMain::DrawGame(const Camera& _camera)
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

	//スカイドームを描画
	sky_->Draw(_camera.GetPos());

	//ステージの描画
	stgMng.Draw(_camera);

	//機体の描画
	machineMng.Draw();

	//アイテムの描画
	itemMng.Draw();

	//プレイヤーの描画
	plMng.Draw();
}

void GameMain::DrawFinish(const Camera& _camera)
{
	//通常描画
	DrawGame(_camera);
}
