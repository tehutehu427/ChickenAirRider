#include"../pch.h"
#include"../Application.h"
#include"../Utility/Utility.h"
#include "../Loader/LoaderManager.h"
#include "../Manager/System/ResourceManager.h"
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
	//ステージ情報
	importData_ = LoaderManager<ImportData>::GetInstance().GetfileData(Utility::WStrToStr(Application::PATH_OUTSIDE + L"Stage.json"));
}

void StageManager::Init(void)
{
	//ステージの作成
	for (const auto& data : importData_)
	{
		//ステージの生成
		stages_.push_back(std::make_unique<StageObject>(modelId_[data.name], data.position, data.scale, data.quaternion));
	}

	//初期化
	for (const auto& stage : stages_)
	{
		stage->Init();
	}
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
	//インスタンス
	auto& res = ResourceManager::GetInstance();

	//リソース
	modelId_["glass"] = res.LoadModelDuplicate(ResourceManager::SRC::GLASS);
	modelId_["building"] = res.LoadModelDuplicate(ResourceManager::SRC::BUILDING);
}

StageManager::~StageManager(void)
{
}
