#include "../../pch.h"
#include "../../Application.h"
#include "../../Common/Vector2.h"
#include "../../Utility/Utility.h"
#include "../System/SceneManager.h"
#include "../System/SplitScreenManager.h"
#include "../System/TextManager.h"
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
	for (int i = 0 ; i < GameSetting::PLAYER_MAX_NUM;i++)
	{
		//中身がないならカウンタも動かさない
		if (type_[i].empty())continue;

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
	//分割スクリーン
	auto& split = SplitScreenManager::GetInstance();
	int activeViewCnt = split.GetActiveViewCount();

	for (int i = 0; i < activeViewCnt; i++)
	{
		DrawGuide(i);
	}
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
	const auto& plIsDisplay = isDisplay_[_playerIndex];
	const bool isDisplay = plIsDisplay[static_cast<int>(_type)];

	//表示しないようになっているなら追加しない
	if (!isDisplay)return;

	//追加
	type_[_playerIndex].push(_type);
}

void GuideManager::SetIsDisplay(const GUIDE_TYPE& _type, const int _playerIndex, const bool _isDisplay)
{
	//表示判定
	auto& plIsDisplay = isDisplay_[_playerIndex];
	auto& isDisplay = plIsDisplay[static_cast<int>(_type)];

	//設定
	isDisplay = _isDisplay;
}

GuideManager::GuideManager(void)
{
	text_[static_cast<int>(GUIDE_TYPE::CHARGE)] = L"Aボタンを押そう！！";
	text_[static_cast<int>(GUIDE_TYPE::SPIN)] = L"";
	text_[static_cast<int>(GUIDE_TYPE::ITEM_BOX)] = L"";
	text_[static_cast<int>(GUIDE_TYPE::POWER_UP_ITEM)] = L"";
	text_[static_cast<int>(GUIDE_TYPE::BATTLE_ITEM)] = L"";
	text_[static_cast<int>(GUIDE_TYPE::GET_OFF)] = L"";
	text_[static_cast<int>(GUIDE_TYPE::MACHINE)] = L"";
}

GuideManager::~GuideManager(void)
{
}

void GuideManager::LoadOutSide(void)
{
}

void GuideManager::DrawGuide(const int _playerIndex)
{
	//表示するものがないなら何もしない
	if (type_[_playerIndex].empty())return;

	//表示するテキスト
	const std::wstring& text = text_[static_cast<int>(type_[_playerIndex].front())];

	//分割
	const auto& view = SplitScreenManager::GetInstance().GetViewport(_playerIndex);

	//サイズ比率(XYほぼ同一なので統一)
	float scale = static_cast<float>(view.height) / Application::SCREEN_SIZE_Y;
	int size = static_cast<int>(TEXT_SIZE * scale);

	//座標
	Vector2 pos = { view.x, view.y };

	//テキスト描画
	const auto& textMng = TextManager::GetInstance();
	textMng.DrawTextToImage(text, pos.x, pos.y, size, TEXT_THICK, Utility::BLACK, FONT_TYPE::BOKUTATI);
}