#include "../pch.h"
#include "../Manager/System/ResourceManager.h"
#include "../Manager/Game/PlayerManager.h"
#include "../Manager/Game/GameSetting.h"
#include "SceneResult.h"

SceneResult::SceneResult(void)
{
}

SceneResult::~SceneResult(void)
{
}

void SceneResult::Load(void)
{
	//インスタンス
	auto& res = ResourceManager::GetInstance();

	//順位画像
	rankImg_.emplace(1, res.Load(ResourceManager::SRC::RANK_ONE).handleId_);
	rankImg_.emplace(2, res.Load(ResourceManager::SRC::RANK_TWO).handleId_);
	rankImg_.emplace(3, res.Load(ResourceManager::SRC::RANK_THREE).handleId_);
	rankImg_.emplace(4, res.Load(ResourceManager::SRC::RANK_FOUR).handleId_);
}

void SceneResult::Init(void)
{
	////インスタンス
	//auto& plMng = PlayerManager::GetInstance();

	////プレイヤー人数
	//int plNum = GameSetting::GetInstance().GetPlayerNum();

	//for (int i = 0; i < plNum; i++)
	//{
	//	Transform trans;
	//	trans.modelId =
	//}
}

void SceneResult::Update(void)
{
}

void SceneResult::Draw(const Camera& _camera)
{
}

void SceneResult::Release(void)
{
}
