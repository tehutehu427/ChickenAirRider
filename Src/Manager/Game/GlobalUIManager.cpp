#include "../pch.h"
#include"../../Application.h"
#include"../Utility/Utility.h"
#include"../Manager/System/ResourceManager.h"
#include"../Manager/Game/Timer.h"
#include"../Manager/Game/GameSetting.h"
#include"../../Renderer/PixelMaterial.h"
#include"../../Renderer/PixelRenderer.h"
#include "GlobalUIManager.h"

void GlobalUIManager::LoadOutSide(void)
{
	//インスタンス
	auto& res = ResourceManager::GetInstance();

	//画像の読み込み
	finishImg_ = res.Load(ResourceManager::SRC::FINISH_UI).handleId_;
	countDownImg_ = res.Load(ResourceManager::SRC::FINISH_NUMBER).handleIds_;

	//マテリアル
	countDownMaterial_ = std::make_unique<PixelMaterial>(L"CountDown.cso", 2);
	countDownMaterial_->AddConstBuf({ 0.0f,0.0f,0.0f,0.0f });
	countDownMaterial_->AddConstBuf({ 0.0f,0.0f,0.0f,0.0f });
	countDownMaterial_->AddTextureBuf(-1);
}

void GlobalUIManager::Init(void)
{
	//インスタンス
	const auto& setting = GameSetting::GetInstance();

	//タイマー
	timer_ = std::make_unique<Timer>();
	timer_->Init(setting.GetTimeLimit());

	//カウント開始
	timer_->SetCountValid(false);
	timer_->SetCountView(false);
}

void GlobalUIManager::Update(void)
{
	//タイマーの更新
	timer_->Update();
}

void GlobalUIManager::Draw(void)
{
	//描画
	for (const auto& draw : draws_)
	{
		//非表示判定
		if (!draw.visible)continue;

		//描画
		(this->*draw.func)();
	}
}

void GlobalUIManager::SetVisible(const DRAW_TYPE _type, const bool _visible)
{
	//表示状況の変更
	draws_[static_cast<int>(_type)].visible = _visible;
}

GlobalUIManager::GlobalUIManager(void)
{
	//描画リスト
	draws_[static_cast<int>(DRAW_TYPE::TIMER)] = { &GlobalUIManager::DrawTimer,false };
	draws_[static_cast<int>(DRAW_TYPE::COUNT_DOWN)] = { &GlobalUIManager::DrawCountDown,false };
	draws_[static_cast<int>(DRAW_TYPE::FINISH)] = { &GlobalUIManager::DrawFinish,false };
	draws_[static_cast<int>(DRAW_TYPE::EVENT)] = { &GlobalUIManager::DrawEvent,false };
}

GlobalUIManager::~GlobalUIManager(void)
{
}

void GlobalUIManager::DrawTimer(void)
{
	//タイマー描画
	timer_->Draw();
}

void GlobalUIManager::DrawCountDown(void)
{
	//影響度
	float time = timer_->GetRemainingTime();
	int number = static_cast<int>(time) % 10 + 1;
	int numberImg = countDownImg_[number];

	//画像分割
	float cellWidth = 1.0f / 5.0f;
	float cellHeight = 1.0f / 2.0f;

	//分割ごとの数字
	int col = number % 5;
	int row = number / 5;

	//UV座標
	Vector2F uvScale = { cellWidth, cellHeight };
	Vector2F uvOffset = { col * cellWidth, row * cellHeight };

	//小数点のみ
	float timeDecimel = time - std::floorf(time);
	float progress = 1.0f - timeDecimel;

	Vector2 size =
	{
		Application::SCREEN_SIZE_X * timeDecimel,
		Application::SCREEN_SIZE_Y * timeDecimel
	};

	Vector2 pos =
	{
		(Application::SCREEN_SIZE_X - size.x) * 0.5f,
		(Application::SCREEN_SIZE_Y - size.y) * 0.5f
	};

	//描画
	countDownMaterial_->SetConstBuf(0, { progress,0.0f,0.0f,0.0f });
	countDownMaterial_->SetConstBuf(1, { uvScale.x,uvScale.y,uvOffset.x,uvOffset.y });
	countDownMaterial_->SetTextureBuf(0, numberImg);
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);
	renderer_.Draw(*countDownMaterial_, pos, size);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
}

void GlobalUIManager::DrawFinish(void)
{
	//枠
	DrawBox(0, Application::SCREEN_HALF_Y - FINISH_FRAME_HEIGHT, Application::SCREEN_SIZE_X, Application::SCREEN_HALF_Y + FINISH_FRAME_HEIGHT, Utility::GRAY, true);

	//フィニッシュUI描画
	DrawRotaGraph(Application::SCREEN_HALF_X, Application::SCREEN_HALF_Y, FINISH_UI_SCALE, 0.0, finishImg_, true);
}

void GlobalUIManager::DrawEvent(void)
{
	//座標
	int posY = Application::SCREEN_SIZE_Y + EVENT_LOCAL_POS_Y;

	//枠
	DrawBox(0, posY - EVENT_FRAME_HEIGHT, Application::SCREEN_SIZE_X, posY + EVENT_FRAME_HEIGHT, 0x000000, true);

	//イベントUI描画
	DrawRotaGraph(Application::SCREEN_HALF_X, posY, EVENT_UI_SCALE, 0.0, eventImg_, true);
}