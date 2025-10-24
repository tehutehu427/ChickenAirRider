#include"../pch.h"
#include "../Object/Stage/StageObject.h"
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

void StageManager::Load(void)
{
}

void StageManager::Init(void)
{
	//ステージの作成
}

void StageManager::Update(void)
{
	//ステージの更新
	for (const auto& stage : stages_)
	{
		stage->Update();
	}
}

void StageManager::Draw(void)
{
	//ステージの描画
	for (const auto& stage : stages_)
	{
		stage->Draw();
	}
}

void StageManager::Destroy(void)
{
	//ステージを明示的に解放
	stages_.clear();
}

StageManager::StageManager(void)
{
	//名前
	name_[OBJECT_TYPE::GLASS] = "glass";
	name_[OBJECT_TYPE::BUILDING] = "building";
}

StageManager::~StageManager(void)
{
}
