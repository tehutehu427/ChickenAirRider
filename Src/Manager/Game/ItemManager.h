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

	//生成の種類
	enum class SPAWN_TYPE
	{
		MAIN,			//基本
		DEATH_MATCH,	//デスマッチ
	};

	//初期化
	void LoadOutSide(void)override;

	//初期化
	void Init(void)override;

	//更新
	void Update(void);

	//描画
	void Draw(void);

	/// @brief パワーアップアイテム生成
	/// @param _pos	生成位置
	/// @param _num 生成数(指定なしならランダム)
	void CreatePowerUpItem(const VECTOR _pos, const int _num = -1);

	/// @brief バトルアイテム生成
	/// @param _pos	生成位置
	/// @param _num 生成数(指定なしならランダム)
	void CreateBattleItem(const VECTOR _pos, const int _num = -1);

	//生成タイプの設定
	void SetSpawnType(const SPAWN_TYPE _type) { type_ = _type; }
	
private:

	//生成相対位置
	static constexpr VECTOR CREATE_MOVE_VEC = { 1.0f,0.0f,1.0f };

	//アイテム生成数
	static constexpr int CREATE_MAX = 4;
	static constexpr int CREATE_MIN = 2;

	//アイテムボックスの生成間隔
	static constexpr float BOX_CREATE_TIME = 5.0f;

	//アイテムボックス生成上限
	static constexpr int BOX_CREATE_MAX = 7;

	//インポートデータ
	std::vector<PowerUpItemImportData> powerUpItemData_;
	std::vector<BattleItemImportData> battleItemData_;
	std::vector<BoxCreatePositionData> boxPosData_;

	//モデルID取得
	std::unordered_map<std::string, std::function<int(void)>> getImageId_;

	//アイテムボックス
	std::vector<std::unique_ptr<ItemBox>> itemBoxes_;
	
	//アイテム
	std::vector<std::unique_ptr<ItemBase>> items_;

	//バトルアイテムの生成
	std::unordered_map<BattleItemBase::BATTLE_ITEM_TYPE, std::function<std::unique_ptr<BattleItemBase>(const VECTOR&, const VECTOR&)>> createBattleItem_;

	//生成タイプ
	SPAWN_TYPE type_;

	//ゲームごとの生成
	std::unordered_map<SPAWN_TYPE, std::function<void(void)>> create_;

	//生成カウンタ
	float boxCreateCnt_;

	//コンストラクタ
	ItemManager(void);
	
	//デストラクタ
	~ItemManager(void)override;

	//生成
	void CreateMain(void);
	void CreateDeathMatch(void);

	//アイテムボックス生成
	void CreateItemBox(void);

	//バトルアイテム生成
	std::unique_ptr<BattleItemBase> CreateCannon(const VECTOR& _pos, const VECTOR& _vec);
};

