#include "../pch.h"
#include"../Application.h"
#include"../Utility/Utility.h"
#include"../Loader/LoaderManager.h"
#include"../Manager/System/ResourceManager.h"
#include"../Manager/System/SceneManager.h"
#include"../Object/Item/ItemBox.h"
#include"../Object/Item/ItemBase.h"
#include"../Object/Item/PowerUpItemBase.h"
#include"../Object/Item/BattleItem/BattleItemBase.h"
#include"../Object/Item/BattleItem/Cannon.h"
#include "ItemManager.h"

void ItemManager::LoadOutSide(void)
{
	//インポートデータ
	powerUpItemData_ = LoaderManager<PowerUpItemImportData>::GetInstance().GetfileData(Utility::WStrToStr(Application::PATH_JSON + L"PowerUpItem.json"));
	battleItemData_ = LoaderManager<BattleItemImportData>::GetInstance().GetfileData(Utility::WStrToStr(Application::PATH_JSON + L"BattleItem.json"));
	boxPosData_ = LoaderManager<BoxCreatePositionData>::GetInstance().GetfileData(Utility::WStrToStr(Application::PATH_JSON + L"BoxCreatePositionData.json"));
}

void ItemManager::Init(void)
{
	items_.clear();
	itemBoxes_.clear();
	type_ = SPAWN_TYPE::MAIN;
}

void ItemManager::Update(void)
{
	//削除された判定を配列から破棄
	std::erase_if(itemBoxes_, [](const std::unique_ptr<ItemBox>& _itemBox) {return _itemBox->IsDead(); });

	for (const auto& itemBox : itemBoxes_)
	{
		//アイテムボックスの更新
		itemBox->Update();
	}

	//削除された判定を配列から破棄
	std::erase_if(items_, [](const std::unique_ptr<ItemBase>& _item) {return _item->IsDead(); });

	for (const auto& item : items_)
	{
		//アイテムの更新
		item->Update();
	}

	//デルタタイム
	const float& delta = SceneManager::GetInstance().GetDeltaTime();

	//カウンタ
	boxCreateCnt_ += delta;

	if (boxCreateCnt_ > BOX_CREATE_TIME)
	{
		//生成
		(this->*create_[static_cast<int>(type_)])();

		//初期化
		boxCreateCnt_ = 0.0f;
	}
}

void ItemManager::Draw(void)
{
	for (auto& itemBox : itemBoxes_)
	{
		//アイテムボックスの描画
		itemBox->Draw();
	}

	for (auto& item : items_)
	{
		//アイテムの描画
		item->Draw();
	}
}

ItemManager::ItemManager(void)
{
	//画像ID

	//パワーアップ
	getImageId_["maxSpeed"] = &ItemManager::GetImageMaxSpeed;
	getImageId_["acceleration"] = &ItemManager::GetImageAcceleration;
	getImageId_["turning"] = &ItemManager::GetImageTurning;
	getImageId_["charge"] = &ItemManager::GetImageCharge;
	getImageId_["flight"] = &ItemManager::GetImageFlight;
	getImageId_["weight"] = &ItemManager::GetImageWeight;
	getImageId_["attack"] = &ItemManager::GetImageAttack;
	getImageId_["defence"] = &ItemManager::GetImageDefence;
	getImageId_["maxHealth"] = &ItemManager::GetImageMaxHealth;

	//パワーアップの種類
	powerUpType_["maxSpeed"] = POWER_UP_TYPE::MAX_SPEED;
	powerUpType_["acceleration"] = POWER_UP_TYPE::ACCELERATION;
	powerUpType_["turning"] = POWER_UP_TYPE::TURNING;
	powerUpType_["charge"] = POWER_UP_TYPE::CHARGE;
	powerUpType_["flight"] = POWER_UP_TYPE::FLIGHT;
	powerUpType_["weight"] = POWER_UP_TYPE::WEIGHT;
	powerUpType_["attack"] = POWER_UP_TYPE::ATTACK;
	powerUpType_["defence"] = POWER_UP_TYPE::DEFENCE;
	powerUpType_["maxHealth"] = POWER_UP_TYPE::MAX_HEALTH;

	//バトルアイテム
	createBattleItem_[static_cast<int>(BattleItemBase::BATTLE_ITEM_TYPE::CANNON)] = &ItemManager::CreateCannon;

	//生成
	create_[static_cast<int>(SPAWN_TYPE::MAIN)] = &ItemManager::CreateMain;
	create_[static_cast<int>(SPAWN_TYPE::DEATH_MATCH)] = &ItemManager::CreateDeathMatch;

	//初期化
	boxCreateCnt_ = 0.0f;
	type_ = SPAWN_TYPE::MAIN;
}

ItemManager::~ItemManager(void)
{
	itemBoxes_.clear();
	items_.clear();
}

void ItemManager::CreateMain(void)
{
	//箱を生成
	CreateItemBox();
}

void ItemManager::CreateDeathMatch(void)
{
	//生成上限ならスキップ
	if (static_cast<int>(itemBoxes_.size()) > BOX_CREATE_MAX - 1)return;

	//ランダム生成
	int rand = Utility::GetRandomValue(0, static_cast<int>(boxPosData_.size()) - 1);
	ItemBox::ITEM_TYPE randType = static_cast<ItemBox::ITEM_TYPE>(Utility::GetRandomValue(0, static_cast<int>(ItemBox::ITEM_TYPE::MAX) - 1));

	//生成
	CreateBattleItem(boxPosData_[rand].pos, boxPosData_[rand].pos, 1);
}

void ItemManager::CreateItemBox(void)
{
	//生成上限ならスキップ
	if (static_cast<int>(itemBoxes_.size()) > BOX_CREATE_MAX - 1)return;

	//ランダム生成
	int rand = Utility::GetRandomValue(0, static_cast<int>(boxPosData_.size()) - 1);
	ItemBox::ITEM_TYPE randType = static_cast<ItemBox::ITEM_TYPE>(Utility::GetRandomValue(0, static_cast<int>(ItemBox::ITEM_TYPE::MAX) - 1));

	//生成済みか
	for (const auto& itemBox : itemBoxes_)
	{
		//生成済みならスキップ
		if (Utility::Equals(itemBox->GetCreatePos(), boxPosData_[rand].pos))return;
	}

	//アイテムボックス
	std::unique_ptr<ItemBox> itemBox = std::make_unique<ItemBox>(boxPosData_[rand].pos,randType);
	itemBox->Load();
	itemBox->Init();

	//格納
	itemBoxes_.push_back(std::move(itemBox));
}

const int ItemManager::GetImageMaxSpeed(void)
{
	return ResourceManager::GetInstance().Load(ResourceManager::SRC::MAX_SPEED).handleId_;
}

const int ItemManager::GetImageAcceleration(void)
{
	return ResourceManager::GetInstance().Load(ResourceManager::SRC::ACCELERATION).handleId_;
}

const int ItemManager::GetImageTurning(void)
{
	return ResourceManager::GetInstance().Load(ResourceManager::SRC::TURNING).handleId_;
}

const int ItemManager::GetImageCharge(void)
{
	return ResourceManager::GetInstance().Load(ResourceManager::SRC::CHARGE).handleId_;
}

const int ItemManager::GetImageFlight(void)
{
	return ResourceManager::GetInstance().Load(ResourceManager::SRC::FLIGHT).handleId_;
}

const int ItemManager::GetImageWeight(void)
{
	return ResourceManager::GetInstance().Load(ResourceManager::SRC::WEIGHT).handleId_;
}

const int ItemManager::GetImageAttack(void)
{
	return ResourceManager::GetInstance().Load(ResourceManager::SRC::ATTACK).handleId_;
}

const int ItemManager::GetImageDefence(void)
{
	return ResourceManager::GetInstance().Load(ResourceManager::SRC::DEFENCE).handleId_;
}

const int ItemManager::GetImageMaxHealth(void)
{
	return ResourceManager::GetInstance().Load(ResourceManager::SRC::MAX_HEALTH).handleId_;
}

std::unique_ptr<BattleItemBase> ItemManager::CreateCannon(const VECTOR& _pos, const VECTOR& _vec)
{
	return std::make_unique<Cannon>(_pos, _vec);
}

void ItemManager::CreatePowerUpItem(const VECTOR& _pos, const VECTOR& _pPos, const int _num)
{
	//サイズ
	int importSize = static_cast<int>(powerUpItemData_.size());

	//プレイヤーから生成位置に向かうベクトル
	VECTOR moveVec = Utility::GetMoveVec(_pPos, _pos);
	
	//ベクトルの差異
	float sideOffset = 0.0f;
	float frontOffset = 0.0f;

	//プレイヤーに対する横ベクトル
	VECTOR side = VCross(moveVec, VGet(0.0f, 1.0f, 0.0f));

	//最終的なベクトル
	VECTOR targetVec = Utility::VECTOR_ZERO;

	//生成数
	int createNum = _num == -1 ? Utility::GetRandomValue(CREATE_MIN, CREATE_MAX) : _num;

	//ランダムでアイテムを決める
	int rand = 0;

	//ランダムで決めた生成数分アイテムを生成する
	for (int i = 0 ; i < createNum ; i++)
	{
		//ランダムで少しベクトルをずらす
		sideOffset = static_cast<float>(Utility::GetRandomValue(-CREATE_SIDE_VEC_OFFSET, CREATE_SIDE_VEC_OFFSET));
		frontOffset = static_cast<float>(Utility::GetRandomValue(0, CREATE_FRONT_VEC_OFFSET));

		//最終的なベクトルを決める
		targetVec = VAdd(VScale(moveVec, frontOffset / Utility::PERCENT), VScale(side, sideOffset / Utility::PERCENT));

		//ランダムでアイテムの種類決め
		rand =  Utility::GetRandomValue(0, importSize - 1);

		//生成
		auto& data = powerUpItemData_[rand];
		std::unique_ptr<ItemBase> item = std::make_unique<PowerUpItemBase>(_pos, targetVec, (this->*getImageId_[data.name])(), data.param, powerUpType_[data.name]);
		item->Load();
		item->Init();

		//配列格納
		items_.push_back(std::move(item));
	}
}

void ItemManager::CreateBattleItem(const VECTOR& _pos, const VECTOR& _pPos, const int _num)
{
	//サイズ
	int importSize = static_cast<int>(battleItemData_.size());

	//プレイヤーから生成位置に向かうベクトル
	VECTOR moveVec = Utility::GetMoveVec(_pPos, _pos);

	//ベクトルの差異
	float sideOffset = 0.0f;
	float frontOffset = 0.0f;

	//プレイヤーに対する横ベクトル
	VECTOR side = VCross(moveVec, VGet(0.0f, 1.0f, 0.0f));

	//最終的なベクトル
	VECTOR targetVec = Utility::VECTOR_ZERO;

	//生成数
	int createNum = _num == -1 ? Utility::GetRandomValue(CREATE_MIN, CREATE_MAX) : _num;

	//ランダムでアイテムを決める
	int rand = 0;

	//ランダムで決めた生成数分アイテムを生成する
	for (int i = 0; i < createNum; i++)
	{
		//ランダムで少しベクトルをずらす
		sideOffset = static_cast<float>(Utility::GetRandomValue(-CREATE_SIDE_VEC_OFFSET, CREATE_SIDE_VEC_OFFSET));
		frontOffset = static_cast<float>(Utility::GetRandomValue(0, CREATE_FRONT_VEC_OFFSET));

		//最終的なベクトルを決める
		targetVec = VAdd(VScale(moveVec, frontOffset / Utility::PERCENT), VScale(side, sideOffset / Utility::PERCENT));

		//ランダムでアイテムの種類決め
		rand = Utility::GetRandomValue(0, importSize - 1);

		//生成
		std::unique_ptr<ItemBase> item = (this->*createBattleItem_[static_cast<int>(battleItemData_[rand].type)])(_pos, targetVec);
		item->Load();
		item->Init();

		//配列格納
		items_.push_back(std::move(item));
	}
}

const std::vector<VECTOR> ItemManager::GetItemBoxPos(void)const
{
	std::vector<VECTOR>ret;

	for (const auto& itemBox : itemBoxes_)
	{
		ret.push_back(itemBox->GetTrans().pos);
	}

	return ret;
}

const std::vector<VECTOR> ItemManager::GetItemPos(void)const
{
	std::vector<VECTOR>ret;

	for (const auto& item : items_)
	{
		ret.push_back(item->GetTrans().pos);
	}

	return ret;
}
