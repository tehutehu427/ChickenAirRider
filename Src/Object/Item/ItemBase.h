#pragma once
#include<memory>
#include "../ObjectBase.h"

class ItemBase : public ObjectBase
{
public:

	//状態
	enum class STATE
	{
		ALIVE,	//生存
		GOT,	//取得
		DEAD,	//死亡
	};

	/// @brief コンストラクタ
	/// @param _pos 生成座標
	/// @param _vec 移動方向
	ItemBase(const VECTOR& _pos,const VECTOR& _vec);

	//デストラクタ
	virtual ~ItemBase(void)override;

	//読み込み
	virtual void Load(void)override;

	//初期化
	virtual void Init(void)override;

	//更新
	virtual void Update(void)override;

	//描画
	virtual void Draw(void)override;

	//当たり判定
	virtual void OnHit(std::weak_ptr<Collider> _hitCol)override;

	//死亡判定の取得
	const bool IsDead(void)const { return state_ == STATE::DEAD; }

protected:

	//コライダ番号
	enum class COL_VALUE
	{
		OBJECT,	//オブジェクト
		PLAYER,	//本体
	};

	//画像の大きさ
	static constexpr float GOT_IMG_SIZE = 64.0f;
	static constexpr float ALIVE_IMG_SIZE = GOT_IMG_SIZE * 3.0f;

	//半径
	static constexpr float OBJECT_HIT_RADIUS = 70.0f;
	static constexpr float PLAYER_HIT_RADIUS = 150.0f;

	//取得後表示相対座標
	static constexpr float LOCAL_HITER_POS_Y = 100.0f;

	//移動力
	static constexpr int MOVE_POW_MIN = 5;
	static constexpr int MOVE_POW_MAX = 10;
	static constexpr float MOVE_POW_Y = 30.0f;

	//コライダ生成時間
	static constexpr float CREATE_COL_TIME = 0.5f;

	//取得後表示カウンタ
	float displayCnt_;
	
	//死亡判定
	STATE state_;

	//移動時間カウンタ
	float createColCnt_;

	//移動方向
	VECTOR vec_;

	//移動量
	VECTOR movePow_;

	//重力
	VECTOR gravPow_;

	//移動後座標
	VECTOR movedPos_;

	//コライダの生成フラグ
	bool isCreateCol_;

	//取得者
	std::weak_ptr<Collider>hiter_;
};

