#include "../pch.h"
#include"../Application.h"
#include"../Utility/Utility.h"
#include"../Loader/LoaderManager.h"
#include"../Manager/System/ResourceManager.h"
#include"../Manager/System/SceneManager.h"
#include"../Object/Item/ItemBox.h"
#include"../Object/Item/ItemBase.h"
#include "ItemManager.h"

void ItemManager::Init(void)
{
	//インポートデータ
	itemData_ = LoaderManager<ItemImportData>::GetInstance().GetfileData(Utility::WStrToStr(Application::PATH_OUTSIDE + L"Item.json"));
	boxPosData_ = LoaderManager<BoxCreatePositionData>::GetInstance().GetfileData(Utility::WStrToStr(Application::PATH_OUTSIDE + L"BoxCreatePositionData.json"));

	CreateItemBox();
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
		CreateItemBox();

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

	//初期化
	boxCreateCnt_ = 0.0f;
}

ItemManager::~ItemManager(void)
{
	itemBoxes_.clear();
	items_.clear();
}

void ItemManager::CreateItemBox(void)
{
	//生成上限ならスキップ
	if (static_cast<int>(itemBoxes_.size()) > BOX_CREATE_MAX - 1)return;

	//ランダム生成
	int rand = Utility::GetRandomValue(0, static_cast<int>(boxPosData_.size()) - 1);

	//生成済みか
	for (const auto& itemBox : itemBoxes_)
	{
		//生成済みならスキップ
		if (Utility::Equals(itemBox->GetCreatePos(), boxPosData_[rand].pos))return;
	}

	//アイテムボックス
	std::unique_ptr<ItemBox> itemBox = std::make_unique<ItemBox>(boxPosData_[rand].pos);
	itemBox->Load();
	itemBox->Init();

	//格納
	itemBoxes_.push_back(std::move(itemBox));
}

void ItemManager::CreateItem(VECTOR _pos)
{
	//サイズ
	int importSize = static_cast<int>(itemData_.size());

	//生成位置
	VECTOR createPos = Utility::VECTOR_ZERO;
	VECTOR localPos = CREATE_LOCAL_POS;

	//生成数
	int createNum = Utility::GetRandomValue(CREATE_MIN, CREATE_MAX);

	//データの値
	int rand = 0;

	//ランダムで決めた生成数分アイテムを生成する
	for (int i = 0 ; i < createNum ; i++)
	{
		//生成位置
		createPos = VAdd(_pos, localPos);

		//位置の反転
		if (i % 2 == 0)localPos.x = -localPos.x;
		else if (i / 2 == 0)localPos.z = -localPos.z;

		//ランダムでアイテムの種類決め
		rand =  Utility::GetRandomValue(0, importSize - 1);

		//生成
		std::unique_ptr<ItemBase> item = std::make_unique<ItemBase>(createPos, itemData_[rand].param, getImageId_[itemData_[rand].name]());
		item->Load();
		item->Init();

		//配列格納
		items_.push_back(std::move(item));
	}
}