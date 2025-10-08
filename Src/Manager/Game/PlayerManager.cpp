#include"../Object/Player/Player.h"
#include "PlayerManager.h"

//静的インスタンスの初期化
PlayerManager* PlayerManager::instance_ = nullptr;

void PlayerManager::CreateInstance(void)
{
	if (instance_ == nullptr)
	{
		instance_ = new PlayerManager();
	}
}

void PlayerManager::Init(void)
{
	//プレイヤーの生成
	players_.emplace_back(std::make_unique<Player>());

	//各プレイヤーの初期化
	for (auto& player : players_)
	{
		player->Load();
		player->Init();
	}
}

void PlayerManager::Update(void)
{
	//各プレイヤーの更新
	for (auto& player : players_)
	{
		//更新
		player->Update();
		
		//当たり判定の整理
		player->Sweep();
	}
}

void PlayerManager::Draw(void)
{
	//各プレイヤーの描画
	for (auto& player : players_)
	{
		player->Draw();
	}
}

void PlayerManager::CreatePlayer(const int _userNum, const int cpuNum)
{
}

PlayerManager::PlayerManager(void)
{
}

PlayerManager::~PlayerManager(void)
{
}
