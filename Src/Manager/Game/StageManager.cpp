#include"../pch.h"
#include "../Object/Stage/Stage.h"
#include "StageManager.h"

//静的インスタンスの初期化
StageManager* StageManager::instance_ = nullptr;

void StageManager::CreateInstance(void)
{
	if (instance_ == nullptr)
	{
		instance_ = new StageManager();
	}
}

void StageManager::Init(void)
{
	//ステージの作成
	stage_ = std::make_unique<Stage>();
	stage_->Load();
	stage_->Init();
}

void StageManager::Update(void)
{
	//ステージの更新
	stage_->Update();
}

void StageManager::Draw(void)
{
	//ステージの描画
	stage_->Draw();
}

void StageManager::Destroy(void)
{
	//ステージを明示的に解放
	stage_.reset();
}

StageManager::StageManager(void)
{
}

StageManager::~StageManager(void)
{
}
