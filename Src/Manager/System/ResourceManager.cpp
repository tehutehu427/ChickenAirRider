#include"../pch.h"
#include "../../Application.h"
#include "Resource.h"
#include "ResourceManager.h"

void ResourceManager::Init(void)
{
	std::wstring PATH_IMG = Application::PATH_IMAGE;
	std::wstring PATH_MDL = Application::PATH_MODEL;
	std::wstring PATH_EFF = Application::PATH_EFFECT;
	std::wstring PATH_SOUND = Application::PATH_SOUND;

	std::unique_ptr<Resource> res;

#pragma region 画像

	//アイテム画像のパス
	std::wstring itemImage = PATH_IMG + L"Item/";

	//タイトル画像のパス
	std::wstring titleImage = PATH_IMG + L"Title/";

	//UI画像のパス
	std::wstring uiImage = PATH_IMG + L"UI/";

	//パワーアップ
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

	//バトルアイテム
	res = std::make_unique<Resource>(Resource::TYPE::IMG, itemImage + L"Cannon.png");
	resourcesMap_.emplace(SRC::CANNON_IMAGE, std::move(res));
	
	//UI
	res = std::make_unique<Resource>(Resource::TYPE::IMG, titleImage + L"Title.png");
	resourcesMap_.emplace(SRC::TITLE_BACK, std::move(res));

	res = std::make_unique<Resource>(Resource::TYPE::IMG, titleImage + L"TitleLogo.png");
	resourcesMap_.emplace(SRC::TITLE_LOGO, std::move(res));

	res = std::make_unique<Resource>(Resource::TYPE::IMG, titleImage + L"PushAnyButton.png");
	resourcesMap_.emplace(SRC::PUSH_ANY_BUTTON, std::move(res));

	res = std::make_unique<Resource>(Resource::TYPE::IMG, uiImage + L"ChargeGauge.png");
	resourcesMap_.emplace(SRC::CHARGE_GAUGE, std::move(res));

	res = std::make_unique<Resource>(Resource::TYPE::IMG, uiImage + L"SpeedGauge.png");
	resourcesMap_.emplace(SRC::SPEED_GAUGE, std::move(res));

	res = std::make_unique<Resource>(Resource::TYPE::IMGS, uiImage + L"Number.png", NUMBER_NUM_X, NUMBER_NUM_Y, NUMBER_SIZE, NUMBER_SIZE);
	resourcesMap_.emplace(SRC::NUMBER, std::move(res));
	
	res = std::make_unique<Resource>(Resource::TYPE::IMG, uiImage + L"Colon.png");
	resourcesMap_.emplace(SRC::COLON, std::move(res));

	res = std::make_unique<Resource>(Resource::TYPE::IMG, uiImage + L"TimerFrame.png");
	resourcesMap_.emplace(SRC::TIMER_FRAME, std::move(res));

#pragma endregion 


#pragma region マスク画像

	res = std::make_unique<Resource>(Resource::TYPE::IMG, uiImage + L"ChargeGaugeMask.png");
	resourcesMap_.emplace(SRC::CHARGE_GAUGE_MASK, std::move(res));

#pragma endregion 


#pragma region モデル
	//キャラクターのパス
	std::wstring charaPath = PATH_MDL + L"Character/";

	res = std::make_unique<Resource>(Resource::TYPE::MODEL, charaPath + L"Chicken.mv1");
	resourcesMap_.emplace(SRC::CHICKEN, std::move(res));

	//マシンのモデル
	std::wstring machinePath = PATH_MDL + L"Machine/";

	res = std::make_unique<Resource>(Resource::TYPE::MODEL, machinePath + L"Wakaba.mv1");
	resourcesMap_.emplace(SRC::WAKABA_STAR, std::move(res));

	res = std::make_unique<Resource>(Resource::TYPE::MODEL, machinePath + L"Earth.mv1");
	resourcesMap_.emplace(SRC::EARTH_STAR, std::move(res));

	res = std::make_unique<Resource>(Resource::TYPE::MODEL, machinePath + L"Battle.mv1");
	resourcesMap_.emplace(SRC::BATTLE_STAR, std::move(res));

	res = std::make_unique<Resource>(Resource::TYPE::MODEL, machinePath + L"Bottle.mv1");
	resourcesMap_.emplace(SRC::BOTTLE_STAR, std::move(res));

	//ステージオブジェクトのパス
	std::wstring stagePath = PATH_MDL + L"StageObject/";

	res = std::make_unique<Resource>(Resource::TYPE::MODEL, stagePath + L"Stage.mv1");
	resourcesMap_.emplace(SRC::STAGE, std::move(res));

	res = std::make_unique<Resource>(Resource::TYPE::MODEL, stagePath + L"Glass.mv1");
	resourcesMap_.emplace(SRC::GLASS, std::move(res));

	res = std::make_unique<Resource>(Resource::TYPE::MODEL, stagePath + L"Building.mv1");
	resourcesMap_.emplace(SRC::BUILDING, std::move(res));

	res = std::make_unique<Resource>(Resource::TYPE::MODEL, stagePath + L"Tree.mv1");
	resourcesMap_.emplace(SRC::TREE, std::move(res));

	//アイテムのパス
	std::wstring itemPath = PATH_MDL + L"Item/";

	res = std::make_unique<Resource>(Resource::TYPE::MODEL, itemPath + L"ItemBox.mv1");
	resourcesMap_.emplace(SRC::ITEM_BOX, std::move(res));

	res = std::make_unique<Resource>(Resource::TYPE::MODEL, itemPath + L"CannonBarrel.mv1");
	resourcesMap_.emplace(SRC::CANNON_MODEL, std::move(res));

	res = std::make_unique<Resource>(Resource::TYPE::MODEL, itemPath + L"CannonShot.mv1");
	resourcesMap_.emplace(SRC::CANNON_SHOT_MODEL, std::move(res));

	//スカイドームのパス
	std::wstring skyDomePath = PATH_MDL + L"SkyDome/";

	res = std::make_unique<Resource>(Resource::TYPE::MODEL, skyDomePath + L"SkyDome.mv1");
	resourcesMap_.emplace(SRC::SKY_DOME, std::move(res));

#pragma endregion 


#pragma region エフェクト

#pragma endregion

#pragma region サウンド

	//BGMのパス
	std::wstring bgmPath = PATH_SOUND + L"BGM/";

	res = std::make_unique<Resource>(Resource::TYPE::SOUND, bgmPath + L"SelectBGM.mp3");
	resourcesMap_.emplace(SRC::SELECT_BGM, std::move(res));

	res = std::make_unique<Resource>(Resource::TYPE::SOUND, bgmPath + L"MainGameBGM.mp3");
	resourcesMap_.emplace(SRC::MAIN_GAME_BGM, std::move(res));

	res = std::make_unique<Resource>(Resource::TYPE::SOUND, bgmPath + L"LastGameBGM.mp3");
	resourcesMap_.emplace(SRC::LAST_GAME_BGM, std::move(res));

	res = std::make_unique<Resource>(Resource::TYPE::SOUND, bgmPath + L"Result.mp3");
	resourcesMap_.emplace(SRC::RESULT_BGM, std::move(res));

	//SEのパス
	std::wstring sePath = PATH_SOUND + L"SE/";

	res = std::make_unique<Resource>(Resource::TYPE::SOUND, sePath + L"Title.mp3");
	resourcesMap_.emplace(SRC::TITLE_SE, std::move(res));

	res = std::make_unique<Resource>(Resource::TYPE::SOUND, sePath + L"Enter.mp3");
	resourcesMap_.emplace(SRC::ENTER_SE, std::move(res));

	res = std::make_unique<Resource>(Resource::TYPE::SOUND, sePath + L"SelectSe.mp3");
	resourcesMap_.emplace(SRC::SELECT_SE, std::move(res));

	res = std::make_unique<Resource>(Resource::TYPE::SOUND, sePath + L"Cancel.mp3");
	resourcesMap_.emplace(SRC::CANCEL_SE, std::move(res));

	res = std::make_unique<Resource>(Resource::TYPE::SOUND, sePath + L"Engine.mp3");
	resourcesMap_.emplace(SRC::ENGINE, std::move(res));

	res = std::make_unique<Resource>(Resource::TYPE::SOUND, sePath + L"Charge.mp3");
	resourcesMap_.emplace(SRC::CHARGE_SE, std::move(res));

	res = std::make_unique<Resource>(Resource::TYPE::SOUND, sePath + L"ChargeMax.mp3");
	resourcesMap_.emplace(SRC::CHARGE_MAX_SE, std::move(res));

	res = std::make_unique<Resource>(Resource::TYPE::SOUND, sePath + L"Boost.mp3");
	resourcesMap_.emplace(SRC::BOOST, std::move(res));

	res = std::make_unique<Resource>(Resource::TYPE::SOUND, sePath + L"Beam.mp3");
	resourcesMap_.emplace(SRC::BEAM, std::move(res));

	res = std::make_unique<Resource>(Resource::TYPE::SOUND, sePath + L"Cannon.mp3");
	resourcesMap_.emplace(SRC::CANNON_SE, std::move(res));

	res = std::make_unique<Resource>(Resource::TYPE::SOUND, sePath + L"Damage.mp3");
	resourcesMap_.emplace(SRC::DAMAGE, std::move(res));

	res = std::make_unique<Resource>(Resource::TYPE::SOUND, sePath + L"GetItem.mp3");
	resourcesMap_.emplace(SRC::GET_ITEM, std::move(res));

	res = std::make_unique<Resource>(Resource::TYPE::SOUND, sePath + L"Heal.mp3");
	resourcesMap_.emplace(SRC::HEAL, std::move(res));

	res = std::make_unique<Resource>(Resource::TYPE::SOUND, sePath + L"Jump.mp3");
	resourcesMap_.emplace(SRC::JUMP, std::move(res));

	res = std::make_unique<Resource>(Resource::TYPE::SOUND, sePath + L"Spin.mp3");
	resourcesMap_.emplace(SRC::SPIN, std::move(res));

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
