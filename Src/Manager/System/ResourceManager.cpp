#include"../pch.h"
#include "../../Application.h"
#include "Resource.h"
#include "ResourceManager.h"

ResourceManager* ResourceManager::instance_ = nullptr;

void ResourceManager::CreateInstance(void)
{
	if (instance_ == nullptr)
	{
		instance_ = new ResourceManager();
	}
	instance_->Init();
}

ResourceManager& ResourceManager::GetInstance(void)
{
	return *instance_;
}

void ResourceManager::Init(void)
{
	std::wstring PATH_IMG = Application::PATH_IMAGE;
	std::wstring PATH_MDL = Application::PATH_MODEL;
	std::wstring PATH_EFF = Application::PATH_EFFECT;
	std::wstring PATH_SOUND_BGM = Application::PATH_SOUND_BGM;
	std::wstring PATH_SOUND_SE = Application::PATH_SOUND_SE;

	std::unique_ptr<Resource> res;

#pragma region 画像

#pragma endregion 


#pragma region マスク画像

#pragma endregion 


#pragma region モデル
	//キャラクターのパス
	std::wstring charaPath = PATH_MDL + L"Character/";

	res = std::make_unique<Resource>(Resource::TYPE::MODEL, charaPath + L"Chicken.mv1");
	resourcesMap_.emplace(SRC::CHICKEN, std::move(res));

	//マシンのモデル
	std::wstring machinePath = PATH_MDL + L"Machine/";

	res = std::make_unique<Resource>(Resource::TYPE::MODEL, machinePath + L"Wakaba.mv1");
	resourcesMap_.emplace(SRC::WAKABA, std::move(res));

	//ステージオブジェクトのパス
	std::wstring stagePath = PATH_MDL + L"StageObject/";

	res = std::make_unique<Resource>(Resource::TYPE::MODEL, stagePath + L"Glass.mv1");
	resourcesMap_.emplace(SRC::GLASS, std::move(res));

#pragma endregion 


#pragma region エフェクト

#pragma endregion

}

void ResourceManager::Release(void)
{
	for (auto& p : loadedMap_)
	{
		p.second.Release();
	}

	loadedMap_.clear();
}

void ResourceManager::Destroy(void)
{
	Release();
	resourcesMap_.clear();
	delete instance_;
}

const Resource& ResourceManager::Load(SRC src)
{
	Resource& res = _Load(src);
	if (res.type_ == Resource::TYPE::NONE)
	{
		return dummy_;
	}
	return res;
}

int ResourceManager::LoadModelDuplicate(SRC src)
{
	Resource& res = _Load(src);
	if (res.type_ == Resource::TYPE::NONE)
	{
		return -1;
	}

	int duId = MV1DuplicateModel(res.handleId_);
	res.duplicateModelIds_.push_back(duId);

	return duId;
}

ResourceManager::ResourceManager(void)
{
}

ResourceManager::~ResourceManager(void)
{
}

Resource& ResourceManager::_Load(SRC src)
{

	// ロード済みチェック
	const auto& lPair = loadedMap_.find(src);
	if (lPair != loadedMap_.end())
	{
		return lPair->second;
	}

	// リソース登録チェック
	const auto& rPair = resourcesMap_.find(src);
	if (rPair == resourcesMap_.end())
	{
		// 登録されていない
		return dummy_;
	}

	// ロード処理
	rPair->second->Load();

	// 念のためコピーコンストラクタ
	loadedMap_.emplace(src, *rPair->second);

	return *rPair->second;

}
