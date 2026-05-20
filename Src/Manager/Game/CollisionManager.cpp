#include"../pch.h"
#include"../Object/Common/Transform.h"
#include"../Object/Common/Geometry/Geometry.h"
#include"../Object/Common/Collider.h"
#include"../Object/ObjectBase.h"
#include"../Utility/Utility.h"
#include "CollisionManager.h"

//静的インスタンスの初期化
CollisionManager* CollisionManager::instance_ = nullptr;

void CollisionManager::CreateInstance(void)
{
	//インスタンスの初期生成
	if (instance_ == nullptr)
	{
		instance_ = new CollisionManager();
	}
}

void CollisionManager::AddCollider(const std::shared_ptr<Collider> _collider)
{
	//コライダの追加
	colliders_.push_back(_collider);
}

void CollisionManager::Sweep(void)
{
	//終了したコライダを削除する
	std::erase_if(colliders_, [](const std::shared_ptr<Collider> _col) {return _col->IsDead(); });
}

void CollisionManager::Update(void)
{
	//コライダが一つもないなら処理を飛ばす
	if (colliders_.empty())return;

	//コライダの数
	const auto& colSize = colliders_.size();

	//当たり判定フレーム
	if (updateFrame_ < COL_UPDATE_FRAME)
	{
		//カウンタ
		updateFrame_++;
		return;
	}

	for (int i = 0; i < colSize - 1; i++)
	{
		//コライダが死亡済みなら
		if (colliders_[i]->IsDead())
		{
			//飛ばす
			continue;
		}

		for (int j = i + 1; j < colSize; j++)
		{
			//コライダが死亡済みなら
			if (colliders_[i]->IsDead() || colliders_[j]->IsDead())
			{
				//飛ばす
				continue;
			}

			//同一所持者をスキップ
			if (&colliders_[i]->GetOwner() == &colliders_[j]->GetOwner())
			{
				//飛ばす
				continue;
			}

			//事前当たり判定
			if (!IsBroudCollision(colliders_[i]->GetOwner(), colliders_[j]->GetOwner()))
			{
				//飛ばす
				continue;
			}

			//当たり判定をするタグか
			if (!IsCollisionTag(*colliders_[i], *colliders_[j]))
			{
				//飛ばす
				continue;
			}

			//当たり判定
			if (IsCollision(*colliders_[i], *colliders_[j]))
			{
				//それぞれの当たった処理
				colliders_[i]->OnHit(colliders_[j]);
				colliders_[j]->OnHit(colliders_[i]);

				//当たった後の処理
				if (!colliders_[i]->IsDead())
				{
					colliders_[i]->GetGeometry().HitAfter();
				}
				if (!colliders_[j]->IsDead())
				{
					colliders_[j]->GetGeometry().HitAfter();
				}
			}
		}
	}
	//カウンタの初期化
	updateFrame_ = 0;
}

void CollisionManager::Destroy(void)
{
	//コライダの全削除
	colliders_.clear();

	//自身のインスタンス削除
	delete instance_;
	instance_ = nullptr;
}

CollisionManager::CollisionManager(void)
{
	updateFrame_ = 0;
}

CollisionManager::~CollisionManager(void)
{
}

const bool CollisionManager::IsBroudCollision(const ObjectBase& _obj1, const ObjectBase& _obj2)
{
	//原点
	const VECTOR& origin = _obj1.GetTrans().pos;

	//座標
	const VECTOR& pos1 = Utility::VECTOR_ZERO;
	const VECTOR& pos2 = VSub(_obj2.GetTrans().pos,origin);

	//距離
	float range = _obj1.GetBroudRadius() + _obj2.GetBroudRadius();
	float sqrDistance = Utility::SqrMagnitudeF(VAdd(pos1, pos2));

	//判定
	return sqrDistance < range * range;
}

const bool CollisionManager::IsCollisionTag(const Collider& _col1, const Collider& _col2) const
{
	//当たり判定しないタグ
	const auto& notColTags1 = _col1.GetNotHitTags();
	const auto& notColTags2 = _col2.GetNotHitTags();

	//2人目の当たり判定しないタグ
	if (_col1.IsIncludeMyTag(notColTags2))
	{
		//1人目のタグが2人目の当たり判定しないタグと同一だった
		return false;
	}

	//1人目の当たり判定しないタグ
	if (_col2.IsIncludeMyTag(notColTags1))
	{
		//2人目のタグが1人目の当たり判定しないタグと同一だった
		return false;
	}

	//当たる
	return true;
}

const bool CollisionManager::IsCollision(const Collider& _col1, const Collider& _col2)const
{
	//1つ目の当たり判定形状
	auto& geo1 = _col1.GetGeometry();
	
	//2つ目の当たり判定形状
	auto& geo2 = _col2.GetGeometry();

	//当たり判定
	bool ret = geo1.IsHit(geo2);
	
	//当たり判定
	return ret;
}