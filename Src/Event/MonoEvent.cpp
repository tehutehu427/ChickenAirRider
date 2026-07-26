#include "../pch.h"
#include "../Manager/System/SceneManager.h"
#include "../Manager/System/SplitScreenManager.h"
#include "../Manager/System/ResourceManager.h"
#include "../Manager/Game/GlobalUIManager.h"
#include "../Manager/Game/GameSetting.h"
#include "MonoEvent.h"

MonoEvent::MonoEvent(void)
{
}

MonoEvent::~MonoEvent(void)
{
}

void MonoEvent::Start(void)
{
	//インスタンス
	SplitScreenManager& split = SplitScreenManager::GetInstance();
	ResourceManager& res = ResourceManager::GetInstance();
	GlobalUIManager& gloUi = GlobalUIManager::GetInstance();

	//初期化
	cnt_ = 0.0f;

	//モノトーンの設定
	for (int i = 0; i < GameSetting::PLAYER_MAX_NUM; i++)
	{
		split.SetShader(i, SplitScreenManager::SHADER_TYPE::MONO);
	}

	//UIの設定
	gloUi.SetVisible(GlobalUIManager::DRAW_TYPE::EVENT, true);
	gloUi.SetEventUI(res.Load(ResourceManager::SRC::MONO_EVENT_UI).handleId_);
}

void MonoEvent::Update(void)
{
	//カウンタの更新
	cnt_ += SceneManager::GetInstance().GetDeltaTime();
	//終了判定
	if (cnt_ >= LIMIT_TIME)
	{
		isEnd_ = true;
	}
}

void MonoEvent::End(void)
{
	//インスタンス
	SplitScreenManager& split = SplitScreenManager::GetInstance();
	GlobalUIManager& gloUi = GlobalUIManager::GetInstance();

	//初期化
	cnt_ = 0.0f;

	//ゴッドレイの設定
	for (int i = 0; i < GameSetting::PLAYER_MAX_NUM; i++)
	{
		split.SetShader(i, SplitScreenManager::SHADER_TYPE::GOD_RAY);
	}

	//UIの設定
	gloUi.SetVisible(GlobalUIManager::DRAW_TYPE::EVENT, false);
}