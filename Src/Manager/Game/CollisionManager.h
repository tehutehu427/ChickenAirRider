#pragma once
#include<vector>
#include<memory>
#include<map>
#include<functional>
#include"../../Object/Common/Collider.h"
#include"../../Object/ObjectBase.h"
#include"../../Common/Singleton.h"

class Geometry;

class CollisionManager : public Singleton<CollisionManager>
{
	//継承元のコンストラクタ等にアクセスするため
	friend class Singleton<CollisionManager>;

public:

	//当たり判定をする範囲
	static constexpr float HIT_RANGE_NORMAL = 1000.0f;	//通常の当たり判定距離
	static constexpr float HIT_RANGE_OBJECT = 2000.0f;	//オブジェクトの当たり判定距離
	static constexpr float HIT_RANGE_GROUND = 35000.0f;	//床の当たり判定距離

	//更新用
	static constexpr int COL_UPDATE_FRAME = 0;		//更新ディレイフレーム
	
	//コライダの追加
	void AddCollider(const std::shared_ptr<Collider> _collider);

	//必要なくなったコライダの削除(更新の最後に置く)
	void Sweep(void);

	//更新
	void Update(void);

	//削除
	void Destroy(void)override;

private:

	//当たり判定格納
	std::vector<std::shared_ptr<Collider>>colliders_;

	//当たり判定するフレーム
	int updateFrame_;

	//コンストラクタ
	CollisionManager(void);

	//デストラクタ
	~CollisionManager(void)override;

	/// <summary>
	/// オブジェクトの距離による事前当たり判定
	/// </summary>
	/// <param name="_obj1">オブジェクト1</param>
	/// <param name="_obj2">オブジェクト2</param>
	/// <returns>true:当たり判定を開始する</returns>
	const bool IsBroudCollision(const ObjectBase& _obj1, const ObjectBase& _obj2);

	/// <summary>
	/// 当たり判定をするタグか
	/// </summary>
	/// <param name="_col1">1つ目のコライダ</param>
	/// <param name="_col2">2つ目のコライダ</param>
	/// <returns>当たり判定するタグだった</returns>
	const bool IsCollisionTag(const Collider& _col1, const Collider& _col2)const;

	/// <summary>
	/// 当たり判定
	/// </summary>
	/// <param name="_col1">1つ目のコライダ</param>
	/// <param name="_col2">2つ目のコライダ</param>
	/// <returns>true:当たった</returns>
	const bool IsCollision(const Collider& _col1, const Collider& _col2)const;
};

