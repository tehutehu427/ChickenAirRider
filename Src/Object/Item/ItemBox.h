#pragma once
#include "../ObjectBase.h"

class ModelMaterial;
class ModelRenderer;

class ItemBox : public ObjectBase
{
public:

	//コンストラクタ
	ItemBox(VECTOR _pos);

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
	static constexpr float HEALTH_MAX = 150.0f;

	//無敵時間
	static constexpr float INVINCIBLE = 0.05f;

	//落下時の回転量
	static constexpr float FALL_ROT = 12.0f;

	//重力影響度
	static constexpr float GRAVITY_POW = 300.0f;

	//テクスチャバッファ
	static constexpr int CRACK_IMG_BUFF = 11;

	//ひび画像
	int crackImg_;

	//マテリアル
	std::unique_ptr<ModelMaterial>material_;

	//レンダラー
	std::unique_ptr<ModelRenderer>renderer_;

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
};