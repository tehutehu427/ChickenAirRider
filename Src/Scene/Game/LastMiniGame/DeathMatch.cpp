#include "../pch.h"
#include "../Application.h"
#include "../Manager/Game/CollisionManager.h"
#include "../Manager/Game/GameSetting.h"
#include "../Manager/Game/StageManager.h"
#include "../Manager/Game/MachineManager.h"
#include "../Manager/Game/PlayerManager.h"
#include "../Manager/Game/ItemManager.h"
#include "../Object/SkyDome/SkyDome.h"
#include "../Object/Player/Player.h"
#include "DeathMatch.h"

DeathMatch::DeathMatch(SceneGame& _parent)
	: LastGameBase(_parent)
{
}

DeathMatch::~DeathMatch(void)
{
}

void DeathMatch::Init(void)
{
	//インスタンス
	auto& setting = GameSetting::GetInstance();
	auto& stgMng = StageManager::GetInstance();
	auto& machineMng = MachineManager::GetInstance();
	auto& plMng = PlayerManager::GetInstance();
	auto& itemMng = ItemManager::GetInstance();

	//各初期化
	plMng.ResetPlayer();
	stgMng.Init();
	machineMng.Init();
	itemMng.Init();

	//共通初期化
	LastGameBase::Init();
}

void DeathMatch::Update(void)
{
	//インスタンス
	auto& setting = GameSetting::GetInstance();
	auto& colMng = CollisionManager::GetInstance();
	auto& stgMng = StageManager::GetInstance();
	auto& machineMng = MachineManager::GetInstance();
	auto& plMng = PlayerManager::GetInstance();
	auto& itemMng = ItemManager::GetInstance();
	auto& key = KeyConfig::GetInstance();

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

	//プレイヤー人数
	const int plNum = setting.GetPlayerNum();

	//プレイヤーの死亡状態で勝敗を決める
	for (int i = 0; i < plNum; i++)
	{
		//プレイヤー
		const auto& pl = plMng.GetPlayer(i);

		//機体から降りたら
		if (pl.GetState() != Player::STATE::RIDE_MACHINE)
		{
			//順位決定
			ConfirmRank(pl.GetPlayerIndex());
		}
	}

	//共通更新
	LastGameBase::Update();
}

void DeathMatch::Draw(void)
{
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

void DeathMatch::Release(void)
{
	LastGameBase::Release();
}
