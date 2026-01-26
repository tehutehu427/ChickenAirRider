#include"../pch.h"
#include"../Application.h"
#include"GameSetting.h"
#include"../Manager/System/SceneManager.h"
#include"../Manager/System/Camera.h"
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
	//インスタンス
	const auto& setting = GameSetting::GetInstance();
	const int userNum = setting.GetUserNum();
	const int plNum = setting.GetPlayerNum();

	//プレイヤーの生成
	for (int i = 0; i < plNum; i++)
	{
		//ユーザーを先に生成
		if (i < userNum)
		{
			CreateUserPlayer(i);
		}
		//ユーザーが終わったらNPCを生成
		else
		{
			CreateNpcPlayer(i);
		}
	}

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
	for (int i = 0 ; i< players_.size() ; i++)
	{
		players_[i]->Draw();
	}
}

void PlayerManager::Destroy(void)
{
	//プレイヤーを明示的に解放
	players_.clear();
}

void PlayerManager::ResetPlayer(const VECTOR _pos)
{
	VECTOR pos = _pos;

	//初期化
	for (auto& player : players_)
	{
		player->Init();
		pos.x += 100.0f;
		player->SetMovedPos(pos);
	}
}

void PlayerManager::PlayerCanGetOff(const bool _getOff)
{
	for (auto& player : players_)
	{
		player->SetCanGetOff(_getOff);
	}
}

PlayerManager::PlayerManager(void)
{
}

PlayerManager::~PlayerManager(void)
{
}

void PlayerManager::CreateUserPlayer(const int _playerIndex)
{
	//インスタンス
	auto& scnMng = SceneManager::GetInstance();

	//プレイヤー
	std::unique_ptr<Player> player = std::make_unique<Player>
		(_playerIndex,
			SceneManager::GetInstance().GetCamera(_playerIndex),
			Player::OPERATION_TYPE::USER,
			static_cast<KeyConfig::JOYPAD_NO>(_playerIndex + 1 ),
			static_cast<Collider::TAG>(_playerIndex)
		);
	players_.emplace_back(std::move(player));

	//カメラ設定
	auto camera = scnMng.GetCamera(_playerIndex).lock();
	camera->SetFollow(&GetPlayer(_playerIndex).GetTrans());
	camera->ChangeMode(Camera::MODE::FOLLOW_LEAP);
}

void PlayerManager::CreateNpcPlayer(const int _playerIndex)
{
	//インスタンス
	auto& scnMng = SceneManager::GetInstance();

	//プレイヤー
	std::unique_ptr<Player> player = std::make_unique<Player>
		(_playerIndex,
			scnMng.GetCamera(0),
			Player::OPERATION_TYPE::NPC,
			static_cast<KeyConfig::JOYPAD_NO>(_playerIndex + 1),
			static_cast<Collider::TAG>(_playerIndex)
		);
	players_.emplace_back(std::move(player));
}
