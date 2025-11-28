#include"../pch.h"
#include "../../Application.h"
#include "Resource.h"
#include "ResourceManager.h"

void ResourceManager::Init(void)
{
	std::wstring PATH_IMG = Application::PATH_IMAGE;
	std::wstring PATH_MDL = Application::PATH_MODEL;
	std::wstring PATH_EFF = Application::PATH_EFFECT;
	std::wstring PATH_SOUND_BGM = Application::PATH_SOUND_BGM;
	std::wstring PATH_SOUND_SE = Application::PATH_SOUND_SE;

	std::unique_ptr<Resource> res;

#pragma region 画像

	//アイテム画像のパス
	std::wstring itemImage = PATH_IMG + L"Item/";

	//タイトル画像のパス
	std::wstring titleImage = PATH_IMG + L"Title/";

	res = std::make_unique<Resource>(Resource::TYPE::IMG, itemImage + L"BoxCrack.png");
	resourcesMap_.emplace(SRC::BOX_CRACK, std::move(res));

	res = std::make_unique<Resource>(Resource::TYPE::IMG, itemImage + L"MaxSpeed.png");
	resourcesMap_.emplace(SRC::MAX_SPEED, std::move(res));

	res = std::make_unique<Resource>(Resource::TYPE::IMG, itemImage + L"Acceleration.png");
	resourcesMap_.emplace(SRC::ACCELERATION, std::move(res));
	
	res = std::make_unique<Resource>(Resource::TYPE::IMG, itemImage + L"Turning.png");
	resourcesMap_.emplace(SRC::TURNING, std::move(res));

	res = std::make_unique<Resource>(Resource::TYPE::IMG, itemImage + L"Charge.png");
	resourcesMap_.emplace(SRC::CHARGE, std::move(res));

	res = std::make_unique<Resource>(Resource::TYPE::IMG, itemImage + L"Flight.png");
	resourcesMap_.emplace(SRC::FLIGHT, std::move(res));

	res = std::make_unique<Resource>(Resource::TYPE::IMG, itemImage + L"Weight.png");
	resourcesMap_.emplace(SRC::WEIGHT, std::move(res));

	res = std::make_unique<Resource>(Resource::TYPE::IMG, itemImage + L"Attack.png");
	resourcesMap_.emplace(SRC::ATTACK, std::move(res));

	res = std::make_unique<Resource>(Resource::TYPE::IMG, itemImage + L"Defence.png");
	resourcesMap_.emplace(SRC::DEFENCE, std::move(res));

	res = std::make_unique<Resource>(Resource::TYPE::IMG, itemImage + L"MaxHealth.png");
	resourcesMap_.emplace(SRC::MAX_HEALTH, std::move(res));
	
	res = std::make_unique<Resource>(Resource::TYPE::IMG, titleImage + L"Title.png");
	resourcesMap_.emplace(SRC::TITLE_BACK, std::move(res));

	res = std::make_unique<Resource>(Resource::TYPE::IMG, titleImage + L"TitleLogo.png");
	resourcesMap_.emplace(SRC::TITLE_LOGO, std::move(res));

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

	res = std::make_unique<Resource>(Resource::TYPE::MODEL, stagePath + L"Stage.mv1");
	resourcesMap_.emplace(SRC::STAGE, std::move(res));

	res = std::make_unique<Resource>(Resource::TYPE::MODEL, stagePath + L"Glass.mv1");
	resourcesMap_.emplace(SRC::GLASS, std::move(res));

	res = std::make_unique<Resource>(Resource::TYPE::MODEL, stagePath + L"Building.mv1");
	resourcesMap_.emplace(SRC::BUILDING, std::move(res));

	//アイテムのパス
	std::wstring itemPath = PATH_MDL + L"Item/";

	res = std::make_unique<Resource>(Resource::TYPE::MODEL, itemPath + L"ItemBox.mv1");
	resourcesMap_.emplace(SRC::ITEM_BOX, std::move(res));

	//スカイドームのパス
	std::wstring skyDomePath = PATH_MDL + L"SkyDome/";

	res = std::make_unique<Resource>(Resource::TYPE::MODEL, skyDomePath + L"SkyDome.mv1");
	resourcesMap_.emplace(SRC::SKY_DOME, std::move(res));

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
