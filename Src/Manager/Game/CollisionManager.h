#pragma once

#include<vector>
#include<memory>
#include<map>
#include<functional>
#include"../../Object/Common/Collider.h"

class Geometry;

class CollisionManager
{
public:

	//当たり判定をする範囲
	static constexpr float HIT_RANGE_NORMAL = 1000.0f;	//通常の当たり判定距離
	static constexpr float HIT_RANGE_OBJECT = 2000.0f;	//通常の当たり判定距離
	static constexpr float HIT_RANGE_GROUND = 35000.0f;	//床の当たり判定距離

	//更新用
	static constexpr int COL_UPDATE_FRAME = 0;		//更新ディレイフレーム
	
	//インスタンス生成
	static void CreateInstance(void);

	//インスタンスの取得
	static CollisionManager& GetInstance(void) { return *instance_; }

	//コライダの追加
	void AddCollider(const std::shared_ptr<Collider> _collider);

	//必要なくなったコライダの削除(更新の最後に置く)
	void Sweep(void);

	//更新
	void Update(void);

	//削除
	void Destroy(void);

private:

	//静的インスタンス
	static CollisionManager* instance_;

	//当たり判定格納
	std::vector<std::shared_ptr<Collider>>colliders_;

	//当たり判定距離の二乗
	std::map<Collider::TAG, float> hitRange_;

	//当たり判定するフレーム
	int updateFrame_;

	//コンストラクタ
	CollisionManager(void);

	//コピーの禁止
	CollisionManager(const CollisionManager& _copy) = delete;
	CollisionManager& operator= (const CollisionManager& _copy) = delete;

	//デストラクタ
	~CollisionManager(void);

	/// @brief 当たり判定距離内にいるか
	/// @param _col1 1つ目のコライダ
	/// @param _col2 2つ目のコライダ
	/// @return true:範囲内
	const bool IsWithInHitRange(const std::weak_ptr<Collider> _col1, const std::weak_ptr<Collider> _col2)const;

	/// @brief 当たり判定をするか
	/// @param _col1Num 1つ目のコライダ番号
	/// @param _col2Num 2つ目のコライダ番号
	/// @return true:当たり判定をする
	const bool JudgeIsCollision(const int _col1Num, const int _col2Num)const;

	/// @brief 当たり判定
	/// @param _col1 1つ目のコライダ
	/// @param _col2 2つ目のコライダ
	/// @return true:当たった
	const bool IsCollision(const std::weak_ptr<Collider> _col1, const std::weak_ptr<Collider> _col2)const;
};

