#pragma once
#include<functional>
#include "../Common/Singleton.h"
#include "../Object/Item/ItemImportData.h"
#include "../Object/Item/PowerUpType.h"
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
		MAX
	};

	//初期化
	void LoadOutSide(void)override;

	//初期化
	void Init(void)override;

	//更新
	void Update(void);

	//描画
	void Draw(void);
	
	/// <summary>
	/// パワーアップアイテム生成
	/// </summary>
	/// <param name="_pos">生成位置</param>
	/// <param name="_pPos">プレイヤーの座標</param>
	/// <param name="_num">生成数(指定なしならランダム)</param>
	void CreatePowerUpItem(const VECTOR& _pos, const VECTOR& _pPos, const int _num = -1);

	/// <summary>
	/// バトルアイテム生成
	/// </summary>
	/// <param name="_pos">生成位置</param>
	/// <param name="_pPos">プレイヤーの座標</param>
	/// <param name="_num">生成数(指定なしならランダム)</param>
	void CreateBattleItem(const VECTOR& _pos, const VECTOR& _pPos, const int _num = -1);

	//生成タイプの設定
	void SetSpawnType(const SPAWN_TYPE _type) { type_ = _type; }

	//アイテムボックスの座標
	const std::vector<VECTOR> GetItemBoxPos(void)const;

	//アイテムの座標
	const std::vector<VECTOR> GetItemPos(void)const;
	
private:

	//アイテム生成数
	static constexpr int CREATE_MAX = 4;
	static constexpr int CREATE_MIN = 2;

	//アイテムの生成ベクトルの差異
	static constexpr int CREATE_SIDE_VEC_OFFSET = 40;
	static constexpr int CREATE_FRONT_VEC_OFFSET = 100;

	//アイテムボックスの生成間隔
	static constexpr float BOX_CREATE_TIME = 5.0f;

	//アイテムボックス生成上限
	static constexpr int BOX_CREATE_MAX = 7;

	//インポートデータ
	std::vector<PowerUpItemImportData> powerUpItemData_;
	std::vector<BattleItemImportData> battleItemData_;
	std::vector<BoxCreatePositionData> boxPosData_;

	//モデルID取得
	using GetImageFunc = const int(ItemManager::*)(void);
	std::unordered_map<std::string, GetImageFunc> getImageId_;
	std::unordered_map<std::string, POWER_UP_TYPE> powerUpType_;

	//アイテムボックス
	std::vector<std::unique_ptr<ItemBox>> itemBoxes_;
	
	//アイテム
	std::vector<std::unique_ptr<ItemBase>> items_;

	//バトルアイテムの生成
	using CreateBattleItemFunc = std::unique_ptr<BattleItemBase>(ItemManager::*)(const VECTOR&, const VECTOR&);
	std::array<CreateBattleItemFunc, static_cast<int>(BattleItemBase::BATTLE_ITEM_TYPE::MAX)> createBattleItem_;

	//生成タイプ
	SPAWN_TYPE type_;

	//ゲームごとの生成
	using CreateFunc = void(ItemManager::*)(void);
	std::array<CreateFunc, static_cast<int>(SPAWN_TYPE::MAX)> create_;

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

	//アイテム画像取得
	const int GetImageMaxSpeed(void);
	const int GetImageAcceleration(void);
	const int GetImageTurning(void);
	const int GetImageCharge(void);
	const int GetImageFlight(void);
	const int GetImageWeight(void);
	const int GetImageAttack(void);
	const int GetImageDefence(void);
	const int GetImageMaxHealth(void);

	//バトルアイテム生成
	std::unique_ptr<BattleItemBase> CreateCannon(const VECTOR& _pos, const VECTOR& _vec);
};