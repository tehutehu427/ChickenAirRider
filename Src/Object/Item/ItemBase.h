#pragma once
#include "../ObjectBase.h"
#include "../Object/Player/Parameter/Parameter.h"

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

	//コンストラクタ
	ItemBase(const VECTOR _pos, const Parameter& _param, const int _imageId);

	//デストラクタ
	~ItemBase(void)override;

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
	const bool IsDead(void)const { return state_ == STATE::DEAD; }

	//パラメーターの取得
	const Parameter& GetParam(void)const { return param_; }

private:

	//画像の大きさ
	static constexpr float GOT_IMG_SIZE = 64.0f;
	static constexpr float ALIVE_IMG_SIZE = GOT_IMG_SIZE * 3.0f;

	//半径
	static constexpr float RADIUS = 150.0f;

	//取得後表示時間
	static constexpr float GOT_DISPLAY_TIME = 1.0f;

	//取得後表示相対座標
	static constexpr float LOCAL_HITER_POS_Y = 100.0f;

	//取得後表示カウンタ
	float displayCnt_;
	
	//死亡判定
	STATE state_;

	//取得者
	std::weak_ptr<Collider>hiter_;

	//パラメーター
	Parameter param_;
};

