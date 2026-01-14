#pragma once
#include "../ObjectBase.h"

class ModelMaterial;
class ModelRenderer;

class ItemBox : public ObjectBase
{
public:

	//アイテムのタイプ
	enum class ITEM_TYPE
	{
		POWER_UP,	//パワーアップアイテム
		BATTLE,		//バトルアイテム
		MAX
	};

	//コンストラクタ
	ItemBox(const VECTOR& _pos, const ITEM_TYPE _type);

	//デストラクタ
	~ItemBox(void)override;

	//読み込み
	void Load(void)override;

	//初期化
	void Init(void)override;

	//更新
	void Update(void)override;

	//描画
	void Draw(void)override;

	//当たり判定
	void OnHit(std::weak_ptr<Collider> _hitCol)override;

	//死亡判定の取得
	const bool IsDead(void)const { return isDead_; }

	//生成座標の取得
	const VECTOR& GetCreatePos(void) { return createPos_; }

private:
	
	//当たり判定
	static constexpr int FOOT_COL = 1;	//足元判定のコライダ番号

	//当たり判定の半分
	static constexpr VECTOR BOX_HALF = { 100.0f,100.0f,100.0f };

	//足元判定
	static constexpr VECTOR LOCAL_LINE_UP = { 0.0f,0.0f,0.0f };				//線判定の上相対座標
	static constexpr VECTOR LOCAL_LINE_DOWN = { 0.0f,-110.0f,0.0f };		//線判定の下相対座標

	//体力の最大値
	static constexpr float HEALTH_MAX = 50.0f;

	//無敵時間
	static constexpr float INVINCIBLE_SPIN = 0.05f;

	//落下時の回転量
	static constexpr float FALL_ROT = 12.0f;

	//重力影響度
	static constexpr float GRAVITY_POW = 300.0f;

	//テクスチャバッファ
	static constexpr int CRACK_IMG_BUFF = 11;

	//色
	static constexpr COLOR_F POWER_UP_COLOR = { 1.0f,1.0f,1.0f,1.0f };
	static constexpr COLOR_F BATTLE_COLOR = { 1.0f,0.4f,0.4f,1.0f };

	//ダメージ補正
	static constexpr float SPIN_DAMAGE_DEF = 0.3f;

	//ひび画像
	int crackImg_;

	//マテリアル
	std::unique_ptr<ModelMaterial>material_;

	//レンダラー
	std::unique_ptr<ModelRenderer>renderer_;

	//アイテムタイプ
	ITEM_TYPE type_;

	//死亡判定
	bool isDead_;

	//生成された座標
	VECTOR createPos_;

	//足元
	Quaternion footLine_;

	//体力
	float health_;

	//無敵時間
	float invincible_;

	//色
	std::unordered_map<ITEM_TYPE, COLOR_F> color_;

	//アイテムの生成
	std::unordered_map<ITEM_TYPE, std::function<void(void)>> createItem_;

	//パワーアップアイテムの生成
	void CreatePowerUpItem(void);
	//バトルアイテムの生成
	void CreateBattleItem(void);
};