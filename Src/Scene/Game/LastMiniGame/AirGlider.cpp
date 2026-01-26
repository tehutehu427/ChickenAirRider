#include "../pch.h"
#include "../Application.h"
#include "../Manager/Game/CollisionManager.h"
#include "../Manager/Game/GameSetting.h"
#include "../Manager/Game/StageManager.h"
#include "../Manager/Game/MachineManager.h"
#include "../Manager/Game/PlayerManager.h"
#include "../Object/SkyDome/SkyDome.h"
#include "../Object/Player/Player.h"
#include "AirGlider.h"

AirGlider::AirGlider(SceneGame& _parent)
	: LastGameBase(_parent)
{
}

AirGlider::~AirGlider(void)
{
}

void AirGlider::Init(void)
{
	//インスタンス
	auto& setting = GameSetting::GetInstance();
	auto& stgMng = StageManager::GetInstance();
	auto& plMng = PlayerManager::GetInstance();

	//各初期化
	plMng.ResetPlayer(START_POS);
	stgMng.Init(StageManager::MODE::AIR_GLIDER,100);

	//共通初期化
	LastGameBase::Init();
}

void AirGlider::Update(void)
{
	//インスタンス
	auto& setting = GameSetting::GetInstance();
	auto& colMng = CollisionManager::GetInstance();
	auto& stgMng = StageManager::GetInstance();
	auto& plMng = PlayerManager::GetInstance();
	auto& key = KeyConfig::GetInstance();

	//ステージ更新
	stgMng.Update();

	//プレイヤーの更新
	plMng.Update();

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

	//プレイヤーの飛距離で勝敗を決める
	for (int i = 0; i < plNum; i++)
	{
		//プレイヤー
		const auto& pl = plMng.GetPlayer(i);

		//移動が出来なくなったら
		if (!pl.GetCanMove())
		{
			//プレイヤー番号と飛距離(Z座標)を保持
			flightDistance_.emplace(pl.GetPlayerIndex(), pl.GetTrans().pos.z);
		}
	}

	//プレイヤーごとの飛距離が決まった
	if (plMng.GetPlayerSize() <= flightDistance_.size())
	{
		//飛距離比較
		CompDistance();
	}

	//共通更新
	LastGameBase::Update();
}

void AirGlider::Draw(const Camera& _camera)
{
	//インスタンス
	auto& setting = GameSetting::GetInstance();
	auto& stgMng = StageManager::GetInstance();
	auto& plMng = PlayerManager::GetInstance();

	//ステージの描画
	stgMng.Draw(_camera);

	//プレイヤーの描画
	plMng.Draw();
}

void AirGlider::Release(void)
{
	LastGameBase::Release();
}

void AirGlider::CompDistance(void)
{
	//インスタンス
	auto& setting = GameSetting::GetInstance();
	auto& plMng = PlayerManager::GetInstance();

	//プレイヤー人数
	const int plNum = setting.GetPlayerNum();

	//順位決定用
	std::unordered_map<int,int> decideRank;

	for (int i = 0; i < plNum; i++)
	{
		//順位
		int rank = plNum;
		for (int j = 0; j < plNum; j++)
		{
			//比較
			if (i != j && flightDistance_[i] > flightDistance_[j])
			{
				//順位を上に
				rank--;
			}
		}

		//順位保存
		decideRank.emplace(rank, i);
	}

	//順位の設定
	for (const auto& plRank : decideRank)
	{
		ConfirmRank(plRank.second);
	}
}