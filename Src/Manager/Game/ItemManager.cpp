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
	powerUpItemData_ = LoaderManager<PowerUpItemImportData>::GetInstance().GetfileData(Utility::WStrToStr(Application::PATH_OUTSIDE + L"PowerUpItem.json"));
	battleItemData_ = LoaderManager<BattleItemImportData>::GetInstance().GetfileData(Utility::WStrToStr(Application::PATH_OUTSIDE + L"BattleItem.json"));
	boxPosData_ = LoaderManager<BoxCreatePositionData>::GetInstance().GetfileData(Utility::WStrToStr(Application::PATH_OUTSIDE + L"BoxCreatePositionData.json"));
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

		//当たり判定の整理
		itemBox->Sweep();
	}

	//削除された判定を配列から破棄
	std::erase_if(items_, [](const std::unique_ptr<ItemBase>& _item) {return _item->IsDead(); });

	for (const auto& item : items_)
	{
		//アイテムの更新
		item->Update();

		//当たり判定の整理
		item->Sweep();
	}

	//デルタタイム
	const float& delta = SceneManager::GetInstance().GetDeltaTime();

	//カウンタ
	boxCreateCnt_ += delta;

	if (boxCreateCnt_ > BOX_CREATE_TIME)
	{
		//生成
		create_[type_]();

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
	getImageId_["maxSpeed"] = [](void)
	{
		return ResourceManager::GetInstance().Load(ResourceManager::SRC::MAX_SPEED).handleId_;
	};
	getImageId_["acceleration"] = [](void)
	{
		return ResourceManager::GetInstance().Load(ResourceManager::SRC::ACCELERATION).handleId_;
	};
	getImageId_["turning"] = [](void)
	{
		return ResourceManager::GetInstance().Load(ResourceManager::SRC::TURNING).handleId_;
	};
	getImageId_["charge"] = [](void)
	{
		return ResourceManager::GetInstance().Load(ResourceManager::SRC::CHARGE).handleId_;
	};
	getImageId_["flight"] = [](void)
	{
		return ResourceManager::GetInstance().Load(ResourceManager::SRC::FLIGHT).handleId_;
	};
	getImageId_["weight"] = [](void)
	{
		return ResourceManager::GetInstance().Load(ResourceManager::SRC::WEIGHT).handleId_;
	};
	getImageId_["attack"] = [](void)
	{
		return ResourceManager::GetInstance().Load(ResourceManager::SRC::ATTACK).handleId_;
	};
	getImageId_["defence"] = [](void)
	{
		return ResourceManager::GetInstance().Load(ResourceManager::SRC::DEFENCE).handleId_;
	};
	getImageId_["maxHealth"] = [](void)
	{
		return ResourceManager::GetInstance().Load(ResourceManager::SRC::MAX_HEALTH).handleId_;
	};

	createBattleItem_[BattleItemBase::BATTLE_ITEM_TYPE::CANNON] = [this](const VECTOR& _pos, const VECTOR& _vec)
	{
		return CreateCannon(_pos, _vec);
	};

	//生成
	create_[SPAWN_TYPE::MAIN] = [this](void)
	{
		CreateMain();
	};
	create_[SPAWN_TYPE::DEATH_MATCH] = [this](void)
	{
		CreateDeathMatch();
	};

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
	CreateBattleItem(boxPosData_[rand].pos, 1);
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

std::unique_ptr<BattleItemBase> ItemManager::CreateCannon(const VECTOR& _pos, const VECTOR& _vec)
{
	return std::make_unique<Cannon>(_pos, _vec);
}

void ItemManager::CreatePowerUpItem(const VECTOR _pos, const int _num)
{
	//サイズ
	int importSize = static_cast<int>(powerUpItemData_.size());

	//生成位置
	VECTOR moveVec = CREATE_MOVE_VEC;

	//生成数
	int createNum = _num == -1 ? Utility::GetRandomValue(CREATE_MIN, CREATE_MAX) : _num;

	//データの値
	int rand = 0;

	//ランダムで決めた生成数分アイテムを生成する
	for (int i = 0 ; i < createNum ; i++)
	{
		//位置の反転
		if (i % 2 == 0)moveVec.x = -moveVec.x;
		else if (i % 2 == 1)moveVec.z = -moveVec.z;

		//一つだけなら中央に
		if (createNum == 1)moveVec = { 0.0f,0.0f,0.0f };

		//ランダムでアイテムの種類決め
		rand =  Utility::GetRandomValue(0, importSize - 1);

		//生成
		std::unique_ptr<ItemBase> item = std::make_unique<PowerUpItemBase>(_pos, moveVec, getImageId_[powerUpItemData_[rand].name](), powerUpItemData_[rand].param);
		item->Load();
		item->Init();

		//配列格納
		items_.push_back(std::move(item));
	}
}

void ItemManager::CreateBattleItem(const VECTOR _pos, const int _num)
{
	//サイズ
	int importSize = static_cast<int>(battleItemData_.size());

	//生成位置
	VECTOR moveVec = CREATE_MOVE_VEC;

	//生成数
	int createNum = _num == -1 ? Utility::GetRandomValue(CREATE_MIN, CREATE_MAX) : _num;

	//データの値
	int rand = 0;

	//ランダムで決めた生成数分アイテムを生成する
	for (int i = 0; i < createNum; i++)
	{
		//位置の反転
		if (i % 2 == 0)moveVec.x = -moveVec.x;
		else if (i % 2 == 1)moveVec.z = -moveVec.z;

		//一つだけなら中央に
		if (createNum == 1)moveVec = { 0.0f,0.0f,0.0f };

		//ランダムでアイテムの種類決め
		rand = Utility::GetRandomValue(0, importSize - 1);

		//生成
		std::unique_ptr<ItemBase> item = createBattleItem_[battleItemData_[rand].type](_pos, moveVec);
		item->Load();
		item->Init();

		//配列格納
		items_.push_back(std::move(item));
	}
}
