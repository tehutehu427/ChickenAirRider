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
	void AddCollider(Collider* _collider);

	//コライダの削除
	void DeleteCollider(Collider* _collider);

	//更新
	void Update(void);

	//削除
	void Destroy(void)override;

private:

	//当たり判定格納
	std::vector<Collider*>colliders_;

	//当たり判定するフレーム
	int updateFrame_;

	//コンストラクタ
	CollisionManager(void);

	//デストラクタ
	~CollisionManager(void)override;

	/// <summary>
	/// コライダーの形状衝突判定
	/// </summary>
	/// <param name="_col1">コライダ１</param>
	/// <param name="_col2">コライダ２</param>
	void CollisionGeometry(Collider* _col1, Collider* _col2);

	/// <summary>
	/// タグごとの判定
	/// </summary>
	/// <param name="_col1">コライダ１</param>
	/// <param name="_col2">コライダ２</param>
	/// <returns>true:当たり判定をする</returns>
	const bool CheckCollisionTags(const Collider* _col1, const Collider* _col2)const;

	/// <summary>
	/// 判定前の処理
	/// </summary>
	/// <param name="_col1">コライダ１</param>
	/// <param name="_col2">コライダ２</param>
	/// <returns>true:当たり判定をする</returns>
	const bool PreCollision(Collider* _col1, Collider* _col2)const;
};

