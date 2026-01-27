#include "../pch.h"
#include"../Manager/Game/Timer.h"
#include"../Manager/Game/GameSetting.h"
#include "GlobalUIManager.h"

void GlobalUIManager::LoadOutSide(void)
{
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
	for (auto& draw : drawView_)
	{
		draw.second();
	}
}

void GlobalUIManager::AddDraw(const DRAW_TYPE _type)
{
	//追加
	drawView_.emplace(_type, drawList_[_type]);
}

void GlobalUIManager::SubDraw(const DRAW_TYPE _type)
{
	//削除
	drawView_.erase(_type);
}

GlobalUIManager::GlobalUIManager(void)
{
	//描画リスト
	drawList_.emplace(DRAW_TYPE::TIMER, [this](void) {DrawTimer(); });
	drawList_.emplace(DRAW_TYPE::LAST_COUNT_DOWN, [this](void) {DrawCountDown(); });
	drawList_.emplace(DRAW_TYPE::FINISH, [this](void) {DrawFinish(); });
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
}

void GlobalUIManager::DrawFinish(void)
{
}
