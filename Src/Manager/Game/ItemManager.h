#pragma once
#include<functional>
#include "../Common/Singleton.h"
#include "../Object/Item/ItemImportData.h"
#include "../Object/Item/BoxCreatePositionData.h"

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

	/// @brief アイテム生成
	/// @param _pos	生成位置
	void CreateItem(VECTOR _pos);
	
private:

	//生成相対位置
	static constexpr VECTOR CREATE_LOCAL_POS = { 300.0f,0.0f,300.0f };

	//アイテム生成数
	static constexpr int CREATE_MAX = 4;
	static constexpr int CREATE_MIN = 1;

	//アイテムボックスの生成間隔
	static constexpr float BOX_CREATE_TIME = 2.0f;

	//インポートデータ
	std::vector<ItemImportData> itemData_;
	std::vector<BoxCreatePositionData> boxPosData_;

	//モデルID取得
	std::unordered_map<std::string, std::function<int(void)>> getImageId_;

	//アイテムボックス
	std::vector<std::unique_ptr<ItemBox>> itemBoxes_;
	
	//アイテム
	std::vector<std::unique_ptr<ItemBase>> items_;

	//生成カウンタ
	float boxCreateCnt_;

	//コンストラクタ
	ItemManager(void);
	
	//デストラクタ
	~ItemManager(void)override;

	//アイテムボックス生成
	void CreateItemBox(void);
};

