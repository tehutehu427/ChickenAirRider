#include"../pch.h"
#include"../Application.h"
#include"../Utility/Utility.h"
#include"../Manager/System/ResourceManager.h"
#include"../Manager/System/KeyConfig.h"
#include"../Manager/System/SoundManager.h"
#include"../Manager/System/SceneManager.h"
#include"../Manager/Game/GameSetting.h"
#include "SceneSelect.h"

SceneSelect::SceneSelect(void)
{
	backImg_ = -1;
	selectTypeNum_ = static_cast<int>(SELECT_TYPE::GAME_START);
	nowSelectType_ = SELECT_TYPE::HOME;
	selectType_ = SELECT_TYPE::GAME_START;
	playerNum_[PLAYER_NUM_SELECT::USER] = 0;
	playerNum_[PLAYER_NUM_SELECT::NPC] = 0;
	playerNumSelect_ = PLAYER_NUM_SELECT::USER;

	//更新
	update_[SELECT_TYPE::HOME] = [this](void)
	{
		UpdateHome();
	};
	update_[SELECT_TYPE::GAME_START] = [this](void)
	{
		UpdateGameStart();
	};
	update_[SELECT_TYPE::OPTION] = [this](void)
	{
		UpdateOption();
	};
	update_[SELECT_TYPE::TITLE] = [this](void)
	{
		UpdateTitle();
	};
	update_[SELECT_TYPE::GAME_END] = [this](void)
	{
		UpdateGameEnd();
	};
	update_[SELECT_TYPE::MAX] = [this](void){};

	//描画
	draw_[SELECT_TYPE::HOME] = [this](void)
	{
		DrawHome();
	};
	draw_[SELECT_TYPE::GAME_START] = [this](void)
	{
		DrawGameStart();
	};
	draw_[SELECT_TYPE::OPTION] = [this](void)
	{
		DrawOption();
	};
	draw_[SELECT_TYPE::TITLE] = [this](void)
	{
		DrawTitle();
	};
	draw_[SELECT_TYPE::GAME_END] = [this](void)
	{
		DrawGameEnd();
	};
	draw_[SELECT_TYPE::MAX] = [this](void){};
}

SceneSelect::~SceneSelect(void)
{
	SoundManager::GetInstance().StopAll();
}

void SceneSelect::Load(void)
{
	//背景
	backImg_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::TITLE_BACK).handleId_;

	//サウンド
	auto& snd = SoundManager::GetInstance();
	auto& res = ResourceManager::GetInstance();

	//追加
	int id = res.Load(ResourceManager::SRC::SELECT_BGM).handleId_;
	snd.Add(SoundManager::SOUND_NAME::SELECT_BGM, id, SoundManager::TYPE::BGM);

	id = res.Load(ResourceManager::SRC::ENTER_SE).handleId_;
	snd.Add(SoundManager::SOUND_NAME::ENTER, id, SoundManager::TYPE::SE, 80);

	id = res.Load(ResourceManager::SRC::SELECT_SE).handleId_;
	snd.Add(SoundManager::SOUND_NAME::SELECT_SE, id, SoundManager::TYPE::SE, 80);

	id = res.Load(ResourceManager::SRC::CANCEL_SE).handleId_;
	snd.Add(SoundManager::SOUND_NAME::CANCEL, id, SoundManager::TYPE::SE, 80);
}

void SceneSelect::Init(void)
{
	//初期化
	selectTypeNum_ = static_cast<int>(SELECT_TYPE::GAME_START);
	selectType_ = SELECT_TYPE::GAME_START;
	nowSelectType_ = SELECT_TYPE::HOME;
	playerNum_[PLAYER_NUM_SELECT::USER] = 1;
	playerNum_[PLAYER_NUM_SELECT::NPC] = 0;
	playerNumSelect_ = PLAYER_NUM_SELECT::USER;

	//インスタンス
	auto& snd = SoundManager::GetInstance();

	//BGM再生
	snd.Play(SoundManager::SOUND_NAME::SELECT_BGM, SoundManager::PLAYTYPE::LOOP);
}

void SceneSelect::Update(void)
{
	//選択した項目
	update_[nowSelectType_]();
}

void SceneSelect::Draw(void)
{
#ifdef _DEBUG

	//デバッグ描画
	DebugDraw();

#endif // _DEBUG

	//背景
	DrawExtendGraph(0, 0, Application::SCREEN_SIZE_X, Application::SCREEN_SIZE_Y, backImg_, true);

	//選択肢ごとの描画
	draw_[nowSelectType_]();
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

void SceneSelect::UpdateHome(void)
{
	//インスタンス
	auto& snd = SoundManager::GetInstance();
	auto& key = KeyConfig::GetInstance();

	//決定
	if (key.IsTrgDown(KeyConfig::CONTROL_TYPE::ENTER, KeyConfig::JOYPAD_NO::PAD1))
	{
		//決定音
		snd.Play(SoundManager::SOUND_NAME::ENTER, SoundManager::PLAYTYPE::BACK);

		//決定
		nowSelectType_ = selectType_;
	}

	//上入力
	if (key.IsTrgDown(KeyConfig::CONTROL_TYPE::SELECT_UP, KeyConfig::JOYPAD_NO::PAD1))
	{
		//選択音
		snd.Play(SoundManager::SOUND_NAME::SELECT_SE, SoundManager::PLAYTYPE::BACK);

		//カウントダウン
		selectTypeNum_ = (selectTypeNum_ - 1 + static_cast<int>(SELECT_TYPE::MAX)) % static_cast<int>(SELECT_TYPE::MAX);
	}
	//下入力
	else if (key.IsTrgDown(KeyConfig::CONTROL_TYPE::SELECT_DOWN, KeyConfig::JOYPAD_NO::PAD1))
	{
		//選択音
		snd.Play(SoundManager::SOUND_NAME::SELECT_SE, SoundManager::PLAYTYPE::BACK);
	
		//カウントアップ
		selectTypeNum_ = (selectTypeNum_ + 1) % static_cast<int>(SELECT_TYPE::MAX);
	}

	//選択肢入れ替え
	selectType_ = static_cast<SELECT_TYPE>(selectTypeNum_ % static_cast<int>(SELECT_TYPE::MAX));
}

void SceneSelect::UpdateGameStart(void)
{
	//インスタンス
	auto& snd = SoundManager::GetInstance();
	auto& key = KeyConfig::GetInstance();
	auto& set = GameSetting::GetInstance();

	//決定
	if (key.IsTrgDown(KeyConfig::CONTROL_TYPE::ENTER, KeyConfig::JOYPAD_NO::PAD1)
		&& playerNum_[PLAYER_NUM_SELECT::USER] > 0)
	{
		//決定音
		snd.Play(SoundManager::SOUND_NAME::ENTER, SoundManager::PLAYTYPE::BACK);
	
		//プレイヤーの作成数
		set.SetUserNum(playerNum_[PLAYER_NUM_SELECT::USER]);
		set.SetNpcNum(playerNum_[PLAYER_NUM_SELECT::NPC]);

		//制限時間
		set.SetTimeLimit(30);

		//シーンの削除
		SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::GAME, true, true);
		return;
	}

	//キャンセル
	if (key.IsTrgDown(KeyConfig::CONTROL_TYPE::CANCEL, KeyConfig::JOYPAD_NO::PAD1))
	{
		//キャンセル音
		snd.Play(SoundManager::SOUND_NAME::CANCEL, SoundManager::PLAYTYPE::BACK);

		//ホームに戻る
		nowSelectType_ = SELECT_TYPE::HOME;
	}

	//上入力
	if (key.IsTrgDown(KeyConfig::CONTROL_TYPE::SELECT_UP, KeyConfig::JOYPAD_NO::PAD1)
		&& playerNum_[playerNumSelect_] < GameSetting::PLAYER_MAX_NUM) //上限
	{
		//選択音
		snd.Play(SoundManager::SOUND_NAME::SELECT_SE, SoundManager::PLAYTYPE::BACK);

		//カウントアップ
		playerNum_[playerNumSelect_] = playerNum_[playerNumSelect_] + 1;

		//NPC人数の補正
		if (playerNum_[PLAYER_NUM_SELECT::NPC] > GameSetting::PLAYER_MAX_NUM - playerNum_[PLAYER_NUM_SELECT::USER])
		{
			playerNum_[PLAYER_NUM_SELECT::NPC] = GameSetting::PLAYER_MAX_NUM - playerNum_[PLAYER_NUM_SELECT::USER];
		}
	}
	//下入力
	else if (key.IsTrgDown(KeyConfig::CONTROL_TYPE::SELECT_DOWN, KeyConfig::JOYPAD_NO::PAD1)
		&& playerNum_[playerNumSelect_] > 0) //下限
	{
		//選択音
		snd.Play(SoundManager::SOUND_NAME::SELECT_SE, SoundManager::PLAYTYPE::BACK);
		
		//カウントダウン
		playerNum_[playerNumSelect_] = playerNum_[playerNumSelect_] - 1;
	}
	//左右入力
	else if (key.IsTrgDown(KeyConfig::CONTROL_TYPE::SELECT_RIGHT, KeyConfig::JOYPAD_NO::PAD1)
		|| key.IsTrgDown(KeyConfig::CONTROL_TYPE::SELECT_LEFT, KeyConfig::JOYPAD_NO::PAD1))
	{
		//選択音
		snd.Play(SoundManager::SOUND_NAME::SELECT_SE, SoundManager::PLAYTYPE::BACK);
		
		//ユーザーとNPC選択の切り替え
		playerNumSelect_ = static_cast<PLAYER_NUM_SELECT>((static_cast<int>(playerNumSelect_) + 1) % static_cast<int>(PLAYER_NUM_SELECT::MAX));
	}
}

void SceneSelect::UpdateOption(void)
{
	//ホームに戻る
	nowSelectType_ = SELECT_TYPE::HOME;
}

void SceneSelect::UpdateTitle(void)
{
	//シーンの削除
	SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::TITLE, true, true);
}

void SceneSelect::UpdateGameEnd(void)
{
	//ループ終了
	Application::GetInstance().LoopEnd();
}

void SceneSelect::DrawHome(void)
{
	//選択肢(デバッグ)
	DrawString(SELECT_POS_X, SELECT_POS_Y, L"GAME START", selectType_ == SELECT_TYPE::GAME_START ? Utility::RED : Utility::WHITE);
	DrawString(SELECT_POS_X, SELECT_POS_Y + SELECT_LOCAL_POS, L"OPTION", selectType_ == SELECT_TYPE::OPTION ? Utility::RED : Utility::WHITE);
	DrawString(SELECT_POS_X, SELECT_POS_Y + SELECT_LOCAL_POS * 2, L"TITLE", selectType_ == SELECT_TYPE::TITLE ? Utility::RED : Utility::WHITE);
	DrawString(SELECT_POS_X, SELECT_POS_Y + SELECT_LOCAL_POS * 3, L"GAME END", selectType_ == SELECT_TYPE::GAME_END ? Utility::RED : Utility::WHITE);
}

void SceneSelect::DrawGameStart(void)
{
	DrawFormatString(PLAYER_NUM_POS_X, PLAYER_NUM_POS_Y, playerNumSelect_ == PLAYER_NUM_SELECT::USER ? Utility::RED : Utility::WHITE, L"プレイヤー人数 = %d", playerNum_[PLAYER_NUM_SELECT::USER]);
	DrawFormatString(PLAYER_NUM_POS_X + PLAYER_NUM_LOCAL_POS, PLAYER_NUM_POS_Y, playerNumSelect_ == PLAYER_NUM_SELECT::NPC ? Utility::RED : Utility::WHITE, L"NPC人数 = %d", playerNum_[PLAYER_NUM_SELECT::NPC]);
}

void SceneSelect::DrawOption(void)
{
}

void SceneSelect::DrawTitle(void)
{
	//選択肢(デバッグ)
	DrawString(SELECT_POS_X, SELECT_POS_Y, L"GAME START", selectType_ == SELECT_TYPE::GAME_START ? Utility::RED : Utility::WHITE);
	DrawString(SELECT_POS_X, SELECT_POS_Y + SELECT_LOCAL_POS, L"OPTION", selectType_ == SELECT_TYPE::OPTION ? Utility::RED : Utility::WHITE);
	DrawString(SELECT_POS_X, SELECT_POS_Y + SELECT_LOCAL_POS * 2, L"TITLE", selectType_ == SELECT_TYPE::TITLE ? Utility::RED : Utility::WHITE);
	DrawString(SELECT_POS_X, SELECT_POS_Y + SELECT_LOCAL_POS * 3, L"GAME END", selectType_ == SELECT_TYPE::GAME_END ? Utility::RED : Utility::WHITE);
}

void SceneSelect::DrawGameEnd(void)
{
}
