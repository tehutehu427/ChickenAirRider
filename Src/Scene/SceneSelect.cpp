#include"../pch.h"
#include"../Application.h"
#include"../Utility/Utility.h"
#include"../Manager/System/ResourceManager.h"
#include"../Manager/System/KeyConfig.h"
#include"../Manager/System/SceneManager.h"
#include"../Manager/Game/PlayerManager.h"
#include "SceneSelect.h"

SceneSelect::SceneSelect(void)
{
	backImg_ = -1;
	selectTypeNum_ = static_cast<int>(SELECT_TYPE::GAME_START);
	selectType_ = SELECT_TYPE::GAME_START;

	selectEnter_[SELECT_TYPE::GAME_START] = [this](void)
	{
		SelectGameStart();
	};
	selectEnter_[SELECT_TYPE::OPTION] = [this](void)
	{
		SelectOption();
	};
	selectEnter_[SELECT_TYPE::GAME_END] = [this](void)
	{
		SelectGameEnd();
	};
	selectEnter_[SELECT_TYPE::MAX] = [this](void){};
}

SceneSelect::~SceneSelect(void)
{
}

void SceneSelect::Load(void)
{
	//背景
	backImg_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::TITLE_BACK).handleId_;
}

void SceneSelect::Init(void)
{
	//初期化
	selectTypeNum_ = static_cast<int>(SELECT_TYPE::GAME_START);
	selectType_ = SELECT_TYPE::GAME_START;
}

void SceneSelect::Update(void)
{
	//決定
	if (KeyConfig::GetInstance().IsTrgDown(KeyConfig::CONTROL_TYPE::ENTER, KeyConfig::JOYPAD_NO::PAD1))
	{
		//選択した項目
		selectEnter_[selectType_]();
	}

	//上入力
	if (KeyConfig::GetInstance().IsTrgDown(KeyConfig::CONTROL_TYPE::SELECT_UP, KeyConfig::JOYPAD_NO::PAD1))
	{
		//カウントダウン
		selectTypeNum_ = (selectTypeNum_ - 1 + static_cast<int>(SELECT_TYPE::MAX)) % static_cast<int>(SELECT_TYPE::MAX);
	}
	//下入力
	else if (KeyConfig::GetInstance().IsTrgDown(KeyConfig::CONTROL_TYPE::SELECT_DOWN, KeyConfig::JOYPAD_NO::PAD1))
	{
		//カウントアップ
		selectTypeNum_ = (selectTypeNum_ + 1) % static_cast<int>(SELECT_TYPE::MAX);
	}
	
	//選択肢入れ替え
	selectType_ = static_cast<SELECT_TYPE>(selectTypeNum_ % static_cast<int>(SELECT_TYPE::MAX));
}

void SceneSelect::Draw(void)
{
#ifdef _DEBUG

	//デバッグ描画
	DebugDraw();

#endif // _DEBUG

	//背景
	DrawExtendGraph(0, 0, Application::SCREEN_SIZE_X, Application::SCREEN_SIZE_Y, backImg_, true);

	//選択肢(デバッグ)
	DrawString(SELECT_POS_X, SELECT_POS_Y, L"GAME START", selectType_ == SELECT_TYPE::GAME_START ? Utility::RED : Utility::WHITE);
	DrawString(SELECT_POS_X, SELECT_POS_Y + SELECT_LOCAL_POS, L"OPTION", selectType_ == SELECT_TYPE::OPTION ? Utility::RED : Utility::WHITE);
	DrawString(SELECT_POS_X, SELECT_POS_Y + SELECT_LOCAL_POS * 2, L"GAME END", selectType_ == SELECT_TYPE::GAME_END ? Utility::RED : Utility::WHITE);
}

void SceneSelect::Release(void)
{
}

void SceneSelect::DebugDraw(void)
{
	//シーン名
	DrawString(0, 0, L"SceneSelect", 0xffffff);

	DrawBox(100, 100, 924, 540, 0x00ff0f, true);
}

void SceneSelect::SelectGameStart(void)
{
	//プレイヤー管理の生成
	PlayerManager::CreateInstance();

	//プレイヤーの作成数
	PlayerManager::GetInstance().SetUserNum(2);
	PlayerManager::GetInstance().SetNpcNum(0);

	//シーンの削除
	SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::GAME, true, true);
	return;
}

void SceneSelect::SelectOption(void)
{
}

void SceneSelect::SelectGameEnd(void)
{
	//ループ終了
	Application::GetInstance().LoopEnd();
}
