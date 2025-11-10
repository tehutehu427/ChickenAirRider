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
	importData_ = LoaderManager<StageImportData>::GetInstance().GetfileData(Utility::WStrToStr(Application::PATH_OUTSIDE + L"Stage.json"));
}

void StageManager::Init(void)
{
	//ステージの作成
	for (const auto& data : importData_)
	{
		//モデルの作成
		int modelId = modelId_[data.name]();

		//ステージの生成
		stages_.emplace_back(std::make_unique<StageObject>(data, modelId, tag_[data.tag]));
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
	//リソース
	modelId_["glass"] = [this](void)
	{
		//インスタンス
		auto& res = ResourceManager::GetInstance();
		return res.LoadModelDuplicate(ResourceManager::SRC::GLASS);
	};
	modelId_["building"] = [this](void)
	{
		//インスタンス
		auto& res = ResourceManager::GetInstance();
		return res.LoadModelDuplicate(ResourceManager::SRC::BUILDING);
	};

	//タグ
	tag_["normalObject"] = Collider::TAG::NORMAL_OBJECT;
	tag_["ground"] = Collider::TAG::GROUND;
}

StageManager::~StageManager(void)
{
}