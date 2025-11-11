#pragma once
#include "../Common/Singleton.h"
#include "../Object/Item/ItemImportData.h"

class ItemBox;
class ItemBase;

class ItemManager : public Singleton<ItemManager>
{
	//継承元のコンストラクタ等にアクセスするため
	friend class Singleton<ItemManager>;

public:

	//初期化
	void Init(void)override;

	//更新
	void Update(void);

	//描画
	void Draw(void);
	
private:

	//インポートデータ
	std::vector<ItemImportData> itemData_;

	//アイテムボックス
	std::vector<std::unique_ptr<ItemBox>> itemBoxes_;
	
	//アイテム
	std::vector<std::unique_ptr<ItemBase>> items_;

	//コンストラクタ
	ItemManager(void);
	
	//デストラクタ
	~ItemManager(void)override;

	//アイテムボックス生成
	void CreateItemBox(void);

	//アイテム生成
	void CreateItem(void);
};

