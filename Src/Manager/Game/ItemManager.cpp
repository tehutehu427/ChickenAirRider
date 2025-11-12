#include "../pch.h"
#include"../Application.h"
#include"../Utility/Utility.h"
#include"../Loader/LoaderManager.h"
#include"../Object/Item/ItemBox.h"
#include"../Object/Item/ItemBase.h"
#include "ItemManager.h"

void ItemManager::Init(void)
{
	//インポートデータ
	itemData_ = LoaderManager<ItemImportData>::GetInstance().GetfileData(Utility::WStrToStr(Application::PATH_OUTSIDE + L"Item.json"));

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
}

ItemManager::~ItemManager(void)
{
	itemBoxes_.clear();
	items_.clear();
}

void ItemManager::CreateItemBox(void)
{
	//アイテムボックス
	std::unique_ptr<ItemBox> itemBox = std::make_unique<ItemBox>(VGet(300.0f,0.0f,300.0f));
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
	int createNum = 4;//Utility::GetRandomValue(2, 4);

	//データの値
	int rand = 0;

	for (int i = 0 ; i < createNum ; i++)
	{
		//生成位置
		createPos = VAdd(_pos, localPos);

		//位置の反転
		if (i % 2 == 0)localPos.x = -localPos.x;
		else if (i / 2 == 0)localPos.z = -localPos.z;

		//ランダムでアイテムの種類決め
		rand = Utility::GetRandomValue(0, importSize - 1);

		std::unique_ptr<ItemBase> item = std::make_unique<ItemBase>(createPos, itemData_[0].param);
		item->Load();
		item->Init();

		items_.push_back(std::move(item));
	}
}