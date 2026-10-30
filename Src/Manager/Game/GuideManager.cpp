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
	//表示判定のリセット
	Reset();
}

void GuideManager::Update(void)
{
	//デルタタイム
	float delta = SceneManager::GetInstance().GetDeltaTime();

	//プレイヤーごと
	for (int i = 0 ; i < GameSetting::PLAYER_MAX_NUM;i++)
	{
		//中身がないならカウンタも動かさない
		if (types_[i].empty())continue;

		//表示判定
		auto& guideDatas = guideDatas_[i];
		auto& guideData = guideDatas[static_cast<int>(types_[i].front())];

		//表示時間を超えたら
		if (guideData.cnt > DISPLAY_TIME)
		{
			//現在要素を抜き出し
			types_[i].pop();

			//カウンタ初期化
			guideData.cnt = 0.0f;
		}
		//条件達成済みなら
		else if(guideData.isClear)
		{
			//カウント
			guideData.cnt += delta;
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

void GuideManager::Reset(void)
{
	//プレイヤーごとのガイドの要素
	for (auto& guideDatas : guideDatas_)
	{
		for (auto& guideData : guideDatas)
		{
			//未達成
			guideData = GuideData();

		}
	}
}

void GuideManager::Add(const GUIDE_TYPE& _type, const int _playerIndex)
{
	//表示判定
	const auto& guideData = guideDatas_[_playerIndex];
	const bool isClear = guideData[static_cast<int>(_type)].isClear;

	//表示ガイド
	auto& types = types_[_playerIndex];

	//達成しているなら何もしない
	if (isClear)return;

	//連続で同じガイドは入れない
	if (!types.empty())
	{
		if (types.front() == _type)return;
	}

	//追加
	types.emplace(_type);
}

void GuideManager::Conpleted(const GUIDE_TYPE& _type, const int _playerIndex)
{
	//表示ガイド
	const auto& types = types_[_playerIndex];
	
	//空なら何もしない
	if (types.empty())return;

	//一つ一つやるために表示しているガイドと一致していないなら完了しない
	if (types.front() != _type)return;

	//表示判定
	auto& guideDatas = guideDatas_[_playerIndex];
	auto& guideData = guideDatas[static_cast<int>(_type)];

	//達成
	guideData.isClear = true;
}

GuideManager::GuideManager(void)
{
	text_[static_cast<int>(GUIDE_TYPE::BASIC)] = L"[基本操作]\n何もボタンを押さなくても\n自動的に前に進むよ。\nLスティックを動かすまたはマウスを動かすで\n方向転換もすることができるよ。";
	text_[static_cast<int>(GUIDE_TYPE::CHARGE)] = L"[チャージ]\nAボタン/Bボタンまたは左クリックで\nチャージ＆ブレーキ！！\nゲージを溜めてボタンを離して\n超ダッシュ！！";
	text_[static_cast<int>(GUIDE_TYPE::SPIN)] = L"[スピン]\nLスティックレバガチャまたはマウスを動かすでスピン攻撃！！\n対戦相手やアイテムボックスに\nダメージを与えることができるよ。";
	text_[static_cast<int>(GUIDE_TYPE::ITEM_BOX)] = L"[アイテムボックス]\n箱に近づいてスピン攻撃！！\n壊すとアイテムが出てくるよ。";
	text_[static_cast<int>(GUIDE_TYPE::POWER_UP_ITEM)] = L"[パワーアップアイテム]\n取るとマシンの性能がアップするよ。";
	text_[static_cast<int>(GUIDE_TYPE::BATTLE_ITEM)] = L"[バトルアイテム]\n取ると相手を攻撃することができるよ。";
	text_[static_cast<int>(GUIDE_TYPE::GET_OFF)] = L"[降りる]\nXボタン/Yボタンまたは右クリック長押しで\nマシンから降りることができるよ。";
	text_[static_cast<int>(GUIDE_TYPE::MACHINE)] = L"[マシン]\n別のマシンに乗り換えてみよう。\nマシンによって性能は様々。\n自身にあったマシンを見つけよう。";
}

GuideManager::~GuideManager(void)
{
}

void GuideManager::LoadOutSide(void)
{
}

void GuideManager::DrawGuide(const int _playerIndex)
{
	//表示ガイド
	const auto& types = types_[_playerIndex];
	
	//表示するものがないなら何もしない
	if (types.empty())return;

	//表示するテキスト
	const auto& frontType = types.front();
	const std::wstring& text = text_[static_cast<int>(frontType)];

	//分割
	const auto& view = SplitScreenManager::GetInstance().GetViewport(_playerIndex);

	//サイズ比率(XYほぼ同一なので統一)
	float scale = static_cast<float>(view.height) / Application::SCREEN_SIZE_Y;
	int size = static_cast<int>(TEXT_SIZE * scale);

	//座標
	Vector2 pos = { view.x, view.y };
	Vector2 localPos = LOCAL_POS * scale;
	pos += localPos;

	//テキスト描画
	const auto& textMng = TextManager::GetInstance();
	textMng.DrawTextToImage(text, pos.x, pos.y, size, TEXT_THICK, Utility::BLACK, FONT_TYPE::LOGO);
}