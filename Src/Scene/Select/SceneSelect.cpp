#include"../pch.h"
#include"../Application.h"
#include"../Utility/Utility.h"
#include"../Manager/System/ResourceManager.h"
#include"../Manager/System/KeyConfig.h"
#include"../Manager/System/SoundManager.h"
#include"../Manager/System/SceneManager.h"
#include"../Manager/Game/GameSetting.h"
#include"../Manager/Game/GlobalUIManager.h"
#include"../Manager/Game/Timer.h"
#include "SceneSelect.h"

SceneSelect::SceneSelect(void)
{
	backImg_ = -1;
	selectTypeNum_ = static_cast<int>(MENU_TYPE::GAME_START);
	nowSelectType_ = MENU_TYPE::HOME;
	selectType_ = MENU_TYPE::GAME_START;
	playerNum_[PLAYER_NUM_SELECT::USER] = 0;
	playerNum_[PLAYER_NUM_SELECT::NPC] = 0;
	playerNumSelect_ = PLAYER_NUM_SELECT::USER;

	//更新
	update_[MENU_TYPE::HOME] = [this](void)
	{
		UpdateHome();
	};
	update_[MENU_TYPE::GAME_START] = [this](void)
	{
		UpdateGameStart();
	};
	update_[MENU_TYPE::OPTION] = [this](void)
	{
		UpdateOption();
	};
	update_[MENU_TYPE::TITLE] = [this](void)
	{
		UpdateTitle();
	};
	update_[MENU_TYPE::GAME_END] = [this](void)
	{
		UpdateGameEnd();
	};
	update_[MENU_TYPE::MAX] = [this](void){};

	//描画
	draw_[MENU_TYPE::HOME] = [this](void)
	{
		DrawHome();
	};
	draw_[MENU_TYPE::GAME_START] = [this](void)
	{
		DrawGameStart();
	};
	draw_[MENU_TYPE::OPTION] = [this](void)
	{
		DrawOption();
	};
	draw_[MENU_TYPE::TITLE] = [this](void)
	{
		DrawTitle();
	};
	draw_[MENU_TYPE::GAME_END] = [this](void)
	{
		DrawGameEnd();
	};
	draw_[MENU_TYPE::MAX] = [this](void){};
}

SceneSelect::~SceneSelect(void)
{
	SoundManager::GetInstance().StopAll();
}

void SceneSelect::Load(void)
{
	//インスタンス
	auto& snd = SoundManager::GetInstance();
	auto& res = ResourceManager::GetInstance();

	//背景
	backImg_ = res.Load(ResourceManager::SRC::TITLE_BACK).handleId_;

	//メニュー
	menuBarImg_ = res.Load(ResourceManager::SRC::MENU_BAR).handleId_;
	menuImg_.emplace(MENU_TYPE::GAME_START, res.Load(ResourceManager::SRC::GAME_START_TEXT).handleId_);
	menuImg_.emplace(MENU_TYPE::OPTION, res.Load(ResourceManager::SRC::OPTION_TEXT).handleId_);
	menuImg_.emplace(MENU_TYPE::TITLE, res.Load(ResourceManager::SRC::GO_TITLE_TEXT).handleId_);
	menuImg_.emplace(MENU_TYPE::GAME_END, res.Load(ResourceManager::SRC::GAME_END_TEXT).handleId_);

	//サウンド
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
	selectTypeNum_ = static_cast<int>(MENU_TYPE::GAME_START);
	selectType_ = MENU_TYPE::GAME_START;
	nowSelectType_ = MENU_TYPE::HOME;
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

void SceneSelect::Draw(const Camera& _camera)
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
		selectTypeNum_ = (selectTypeNum_ - 1 + static_cast<int>(MENU_TYPE::MAX)) % static_cast<int>(MENU_TYPE::MAX);
	}
	//下入力
	else if (key.IsTrgDown(KeyConfig::CONTROL_TYPE::SELECT_DOWN, KeyConfig::JOYPAD_NO::PAD1))
	{
		//選択音
		snd.Play(SoundManager::SOUND_NAME::SELECT_SE, SoundManager::PLAYTYPE::BACK);
	
		//カウントアップ
		selectTypeNum_ = (selectTypeNum_ + 1) % static_cast<int>(MENU_TYPE::MAX);
	}

	//選択肢入れ替え
	selectType_ = static_cast<MENU_TYPE>(selectTypeNum_ % static_cast<int>(MENU_TYPE::MAX));
}

void SceneSelect::UpdateGameStart(void)
{
	//インスタンス
	auto& scnMng = SceneManager::GetInstance();
	auto& snd = SoundManager::GetInstance();
	auto& key = KeyConfig::GetInstance();
	auto& set = GameSetting::GetInstance();
	auto& gloUi = GlobalUIManager::GetInstance();

	//決定
	if (key.IsTrgDown(KeyConfig::CONTROL_TYPE::ENTER, KeyConfig::JOYPAD_NO::PAD1)
		&& playerNum_[PLAYER_NUM_SELECT::USER] > 0)
	{
		//決定音
		snd.Play(SoundManager::SOUND_NAME::ENTER, SoundManager::PLAYTYPE::BACK);
	
		//プレイヤーの作成数
		set.SetUserNum(playerNum_[PLAYER_NUM_SELECT::USER]);
		set.SetNpcNum(playerNum_[PLAYER_NUM_SELECT::NPC]);
		
		//タイマーの初期化
		gloUi.GetTimer().Init(set.GetTimeLimit());

		//シーンの削除
		scnMng.ChangeScene(SceneManager::SCENE_ID::GAME, true, true);
		return;
	}

	//キャンセル
	if (key.IsTrgDown(KeyConfig::CONTROL_TYPE::CANCEL, KeyConfig::JOYPAD_NO::PAD1))
	{
		//キャンセル音
		snd.Play(SoundManager::SOUND_NAME::CANCEL, SoundManager::PLAYTYPE::BACK);

		//ホームに戻る
		nowSelectType_ = MENU_TYPE::HOME;
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
	//else if (key.IsTrgDown(KeyConfig::CONTROL_TYPE::SELECT_RIGHT, KeyConfig::JOYPAD_NO::PAD1)
	//	|| key.IsTrgDown(KeyConfig::CONTROL_TYPE::SELECT_LEFT, KeyConfig::JOYPAD_NO::PAD1))
	//{
	//	//選択音
	//	snd.Play(SoundManager::SOUND_NAME::SELECT_SE, SoundManager::PLAYTYPE::BACK);
	//	
	//	//ユーザーとNPC選択の切り替え
	//	playerNumSelect_ = static_cast<PLAYER_NUM_SELECT>((static_cast<int>(playerNumSelect_) + 1) % static_cast<int>(PLAYER_NUM_SELECT::MAX));
	//}
}

void SceneSelect::UpdateOption(void)
{
	//オプションシーンに遷移
	SceneManager::GetInstance().PushScene(SceneManager::SCENE_ID::OPTION);

	//ホームに戻る
	nowSelectType_ = MENU_TYPE::HOME;
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
	//ゲームスタート
	if (selectType_ == MENU_TYPE::GAME_START)SetDrawBright(255, 255, 0);
	DrawRotaGraph(SELECT_POS_X, SELECT_POS_Y, SELECT_SCALE_DEFAULT, 0.0, menuBarImg_, true);
	DrawRotaGraph(SELECT_POS_X, SELECT_POS_Y, SELECT_SCALE_DEFAULT, 0.0, menuImg_[MENU_TYPE::GAME_START], true);
	SetDrawBright(255, 255, 255);

	//オプション
	if (selectType_ == MENU_TYPE::OPTION)SetDrawBright(255, 255, 0);
	DrawRotaGraph(SELECT_POS_X, SELECT_POS_Y + SELECT_LOCAL_POS, SELECT_SCALE_DEFAULT, 0.0, menuBarImg_, true);
	DrawRotaGraph(SELECT_POS_X, SELECT_POS_Y + SELECT_LOCAL_POS, SELECT_SCALE_DEFAULT, 0.0, menuImg_[MENU_TYPE::OPTION], true);
	SetDrawBright(255, 255, 255);

	//タイトルへ
	if (selectType_ == MENU_TYPE::TITLE)SetDrawBright(255, 255, 0);
	DrawRotaGraph(SELECT_POS_X, SELECT_POS_Y + SELECT_LOCAL_POS * 2, SELECT_SCALE_DEFAULT, 0.0, menuBarImg_, true);
	DrawRotaGraph(SELECT_POS_X, SELECT_POS_Y + SELECT_LOCAL_POS * 2, SELECT_SCALE_DEFAULT, 0.0, menuImg_[MENU_TYPE::TITLE], true);
	SetDrawBright(255, 255, 255);

	//ゲーム終了
	if (selectType_ == MENU_TYPE::GAME_END)SetDrawBright(255, 255, 0);
	DrawRotaGraph(SELECT_POS_X, SELECT_POS_Y + SELECT_LOCAL_POS * 3, SELECT_SCALE_DEFAULT, 0.0, menuBarImg_, true);
	DrawRotaGraph(SELECT_POS_X, SELECT_POS_Y + SELECT_LOCAL_POS * 3, SELECT_SCALE_DEFAULT, 0.0, menuImg_[MENU_TYPE::GAME_END], true);
	SetDrawBright(255, 255, 255);
}

void SceneSelect::DrawGameStart(void)
{
	DrawFormatString(PLAYER_NUM_POS_X, PLAYER_NUM_POS_Y, playerNumSelect_ == PLAYER_NUM_SELECT::USER ? Utility::RED : Utility::WHITE, L"プレイヤー人数 = %d", playerNum_[PLAYER_NUM_SELECT::USER]);
	//DrawFormatString(PLAYER_NUM_POS_X + PLAYER_NUM_LOCAL_POS, PLAYER_NUM_POS_Y, playerNumSelect_ == PLAYER_NUM_SELECT::NPC ? Utility::RED : Utility::WHITE, L"NPC人数 = %d", playerNum_[PLAYER_NUM_SELECT::NPC]);
}

void SceneSelect::DrawOption(void)
{
}

void SceneSelect::DrawTitle(void)
{
	//選択肢(デバッグ)
	DrawString(SELECT_POS_X, SELECT_POS_Y, L"GAME START", selectType_ == MENU_TYPE::GAME_START ? Utility::RED : Utility::WHITE);
	DrawString(SELECT_POS_X, SELECT_POS_Y + SELECT_LOCAL_POS, L"OPTION", selectType_ == MENU_TYPE::OPTION ? Utility::RED : Utility::WHITE);
	DrawString(SELECT_POS_X, SELECT_POS_Y + SELECT_LOCAL_POS * 2, L"TITLE", selectType_ == MENU_TYPE::TITLE ? Utility::RED : Utility::WHITE);
	DrawString(SELECT_POS_X, SELECT_POS_Y + SELECT_LOCAL_POS * 3, L"GAME END", selectType_ == MENU_TYPE::GAME_END ? Utility::RED : Utility::WHITE);
}

void SceneSelect::DrawGameEnd(void)
{
}
