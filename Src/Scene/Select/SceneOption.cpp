#include "../pch.h"
#include "../Application.h"
#include "../Utility/Utility.h"
#include "../Manager/System/SceneManager.h"
#include "../Manager/System/KeyConfig.h"
#include "../Manager/System/SoundManager.h"
#include "../Manager/System/ResourceManager.h"
#include "../Manager/Game/GameSetting.h"
#include "SceneOption.h"

SceneOption::SceneOption(void)
{
	//更新
	update_.emplace(OPTION_TYPE::HOME, [this](void) {UpdateHome(); });
	update_.emplace(OPTION_TYPE::TIME, [this](void) {UpdateTime(); });
	update_.emplace(OPTION_TYPE::SCREEN, [this](void) {UpdateScreen(); });
	update_.emplace(OPTION_TYPE::KEY_TYPE, [this](void) {UpdateKeyType(); });
	update_.emplace(OPTION_TYPE::END, [this](void) {UpdateEnd(); });

	//描画
	draw_.emplace(OPTION_TYPE::HOME, [this](void) {DrawHome(); });
	draw_.emplace(OPTION_TYPE::TIME, [this](void) {DrawTime(); });
	draw_.emplace(OPTION_TYPE::SCREEN, [this](void) {DrawScreen(); });
	draw_.emplace(OPTION_TYPE::KEY_TYPE, [this](void) {DrawKeyType(); });
	draw_.emplace(OPTION_TYPE::END, [this](void) {DrawEnd(); });

	//初期化
	backImg_ = -1;
	type_ = OPTION_TYPE::HOME;
	nowType_ = OPTION_TYPE::HOME;
	selectNum_ = -1;

	oldTimeLimit_ = 0;
	oldScreenSize_ = false;
	oldKeyType_ = 0;
	modiTimeLimit_ = 0;
	modiScreenSize_ = false;
	modiKeyType_ = 0;
	timeMulti_ = 0;
}

SceneOption::~SceneOption(void)
{
}

void SceneOption::Load(void)
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

void SceneOption::Init(void)
{
	//初期化
	type_ = OPTION_TYPE::HOME;
	nowType_ = OPTION_TYPE::HOME;
	selectNum_ = 0;

	//インスタンス
	const auto& setting = GameSetting::GetInstance();

	oldTimeLimit_ = setting.GetTimeLimit();
	oldScreenSize_ = setting.GetScreenSize();
	oldKeyType_ = 0;
	modiTimeLimit_ = oldTimeLimit_;
	modiScreenSize_ = oldScreenSize_;
	modiKeyType_ = oldKeyType_;
	timeMulti_ = TIME_SEC_MULTI;
}

void SceneOption::Update(void)
{
	//更新
	update_[nowType_]();
}

void SceneOption::Draw(const Camera& _camera)
{
	//描画
	draw_[nowType_]();

	//背景
	DrawExtendGraph(0, 0, Application::SCREEN_SIZE_X, Application::SCREEN_SIZE_Y, backImg_, true);

	//選択肢(デバッグ)
	DrawString(SELECT_POS_X, SELECT_POS_Y, L"TIME_LIMIT", type_ == OPTION_TYPE::TIME ? Utility::RED : Utility::WHITE);
	DrawString(SELECT_POS_X, SELECT_POS_Y + SELECT_LOCAL_POS_Y, L"FULLSCREEN", type_ == OPTION_TYPE::SCREEN ? Utility::RED : Utility::WHITE);
	DrawString(SELECT_POS_X, SELECT_POS_Y + SELECT_LOCAL_POS_Y * 2, L"KEY_TYPE", type_ == OPTION_TYPE::KEY_TYPE ? Utility::RED : Utility::WHITE);
	DrawString(SELECT_POS_X, SELECT_POS_Y + SELECT_LOCAL_POS_Y * 3, L"END", type_ == OPTION_TYPE::END ? Utility::RED : Utility::WHITE);
	
	DrawFormatString(SELECT_POS_X + SELECT_LOCAL_POS_X, SELECT_POS_Y, type_ == OPTION_TYPE::TIME ? Utility::RED : Utility::WHITE, L"%.2d:%.2d",modiTimeLimit_ / TIME_MIN_MULTI, modiTimeLimit_ % TIME_MIN_MULTI);
	DrawString(SELECT_POS_X + SELECT_LOCAL_POS_X, SELECT_POS_Y + SELECT_LOCAL_POS_Y, modiScreenSize_ == true ? L"OFF" : L"ON", type_ == OPTION_TYPE::SCREEN ? Utility::RED : Utility::WHITE);
	DrawFormatString(SELECT_POS_X + SELECT_LOCAL_POS_X, SELECT_POS_Y + SELECT_LOCAL_POS_Y * 2, type_ == OPTION_TYPE::KEY_TYPE ? Utility::RED : Utility::WHITE, L"KEY_TYPE");
}

void SceneOption::Release(void)
{
}

void SceneOption::UpdateHome(void)
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
		nowType_ = type_;
	}

	//上入力
	if (key.IsTrgDown(KeyConfig::CONTROL_TYPE::SELECT_UP, KeyConfig::JOYPAD_NO::PAD1)
		|| key.IsTrgHold(KeyConfig::CONTROL_TYPE::SELECT_UP, KeyConfig::JOYPAD_NO::PAD1, HOLD_TIME, true))
	{
		//選択音
		snd.Play(SoundManager::SOUND_NAME::SELECT_SE, SoundManager::PLAYTYPE::BACK);

		//カウントダウン
		selectNum_ = (selectNum_ - 1 + static_cast<int>(OPTION_TYPE::MAX)) % static_cast<int>(OPTION_TYPE::MAX);
	}
	//下入力
	else if (key.IsTrgDown(KeyConfig::CONTROL_TYPE::SELECT_DOWN, KeyConfig::JOYPAD_NO::PAD1)
		|| key.IsTrgHold(KeyConfig::CONTROL_TYPE::SELECT_DOWN, KeyConfig::JOYPAD_NO::PAD1, HOLD_TIME, true))
	{
		//選択音
		snd.Play(SoundManager::SOUND_NAME::SELECT_SE, SoundManager::PLAYTYPE::BACK);

		//カウントアップ
		selectNum_ = (selectNum_ + 1) % static_cast<int>(OPTION_TYPE::MAX);
	}

	//選択肢入れ替え
	type_ = static_cast<OPTION_TYPE>(selectNum_ % static_cast<int>(OPTION_TYPE::MAX));
}

void SceneOption::UpdateTime(void)
{
	//インスタンス
	auto& snd = SoundManager::GetInstance();
	auto& key = KeyConfig::GetInstance();

	//決定
	if (key.IsTrgDown(KeyConfig::CONTROL_TYPE::ENTER, KeyConfig::JOYPAD_NO::PAD1))
	{
		//決定音
		snd.Play(SoundManager::SOUND_NAME::ENTER, SoundManager::PLAYTYPE::BACK);

		//確定
		oldTimeLimit_ = modiTimeLimit_;

		//ホームに戻る
		nowType_ = OPTION_TYPE::HOME;
	}
	else if (key.IsTrgDown(KeyConfig::CONTROL_TYPE::CANCEL, KeyConfig::JOYPAD_NO::PAD1))
	{
		//キャンセル音
		snd.Play(SoundManager::SOUND_NAME::CANCEL, SoundManager::PLAYTYPE::BACK);

		//元に戻す
		modiTimeLimit_ = oldTimeLimit_;

		//ホームに戻る
		nowType_ = OPTION_TYPE::HOME;
	}

	//上入力
	if (key.IsTrgDown(KeyConfig::CONTROL_TYPE::SELECT_UP, KeyConfig::JOYPAD_NO::PAD1)
		|| key.IsTrgHold(KeyConfig::CONTROL_TYPE::SELECT_UP, KeyConfig::JOYPAD_NO::PAD1, HOLD_TIME, true))
	{
		//選択音
		snd.Play(SoundManager::SOUND_NAME::SELECT_SE, SoundManager::PLAYTYPE::BACK);

		//カウントアップ
		modiTimeLimit_ += timeMulti_;

		//制限
		if (modiTimeLimit_ > TIME_LIMIT_MAX)
		{
			modiTimeLimit_ = TIME_LIMIT_MAX;
		}
	}
	//下入力
	else if (key.IsTrgDown(KeyConfig::CONTROL_TYPE::SELECT_DOWN, KeyConfig::JOYPAD_NO::PAD1)
		|| key.IsTrgHold(KeyConfig::CONTROL_TYPE::SELECT_DOWN, KeyConfig::JOYPAD_NO::PAD1, HOLD_TIME,true))
	{
		//選択音
		snd.Play(SoundManager::SOUND_NAME::SELECT_SE, SoundManager::PLAYTYPE::BACK);

		//カウントダウン
		modiTimeLimit_ -= timeMulti_;

		//制限
		if (modiTimeLimit_ < TIME_LIMIT_MIN)
		{
			modiTimeLimit_ = TIME_LIMIT_MIN;
		}
	}
	//左右入力
	else if (key.IsTrgDown(KeyConfig::CONTROL_TYPE::SELECT_RIGHT, KeyConfig::JOYPAD_NO::PAD1)
		|| key.IsTrgDown(KeyConfig::CONTROL_TYPE::SELECT_LEFT, KeyConfig::JOYPAD_NO::PAD1))
	{
		//選択音
		snd.Play(SoundManager::SOUND_NAME::SELECT_SE, SoundManager::PLAYTYPE::BACK);

		//倍率変化
		timeMulti_ = timeMulti_ == TIME_SEC_MULTI ? TIME_MIN_MULTI : TIME_SEC_MULTI;
	}
}

void SceneOption::UpdateScreen(void)
{
	//スクリーン変更
	modiScreenSize_ = modiScreenSize_ == true ? false : true;

	//ホームに戻る
	nowType_ = OPTION_TYPE::HOME;
}

void SceneOption::UpdateKeyType(void)
{
	//ホームに戻る
	nowType_ = OPTION_TYPE::HOME;
}

void SceneOption::UpdateEnd(void)
{
	//インスタンス
	auto& setting = GameSetting::GetInstance();

	//要素の確定
	setting.SetTimeLimit(modiTimeLimit_);
	setting.SetScreenSize(modiScreenSize_);

	//セレクトシーンに戻す
	SceneManager::GetInstance().PopScene();
}

void SceneOption::DrawHome(void)
{
}

void SceneOption::DrawTime(void)
{
}

void SceneOption::DrawScreen(void)
{
}

void SceneOption::DrawKeyType(void)
{
}

void SceneOption::DrawEnd(void)
{
}
