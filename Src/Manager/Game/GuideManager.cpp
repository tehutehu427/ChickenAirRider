#include "../../pch.h"
#include "../System/SceneManager.h"
#include "GuideManager.h"

void GuideManager::Init(void)
{
	//カウンタ
	for (auto& cnt : cnt_)
	{
		cnt = 0.0f;
	}

	//表示判定のリセット
	ResetIsDisplay();
}

void GuideManager::Update(void)
{
	//デルタタイム
	float delta = SceneManager::GetInstance().GetDeltaTime();

	//プレイヤーごと
	for (int i = 0 ; 0 < GameSetting::PLAYER_MAX_NUM;i++)
	{
		//表示時間を超えたら
		if (cnt_[i] > DISPLAY_TIME)
		{
			//現在要素を抜き出し
			type_[i].pop();

			//カウンタ初期化
			cnt_[i] = 0.0f;
		}
		else
		{
			//カウント
			cnt_[i] += delta;
		}
	}
}

void GuideManager::Draw(void)
{
}

void GuideManager::ResetIsDisplay(void)
{
	//プレイヤーごとの表示の有無
	for (auto& plDisplay : isDisplay_)
	{
		//要素ごとの表示の有無
		for (auto& isDisplay : plDisplay)
		{
			//表示する
			isDisplay = true;
		}
	}
}

void GuideManager::Add(const GUIDE_TYPE& _type, const int _playerIndex)
{
	//表示判定
	auto& plIsDisplay = isDisplay_[_playerIndex];
	auto& isDisplay = plIsDisplay[static_cast<int>(_type)];

	//表示しないようになっているなら追加しない
	if (!isDisplay)return;

	//追加
	type_[_playerIndex].push(_type);
}

GuideManager::GuideManager(void)
{
}

GuideManager::~GuideManager(void)
{
}

void GuideManager::LoadOutSide(void)
{
}