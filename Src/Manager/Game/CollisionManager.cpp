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

void CollisionManager::AddCollider(Collider* _collider)
{
	//コライダの追加
	colliders_.push_back(_collider);
}

void CollisionManager::Sweep(void)
{
	//終了したコライダを削除する
	std::erase_if(colliders_, [](const Collider* _col) {return _col->IsDead(); });
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
		//コライダ
		auto& coli = colliders_[i];

		//コライダが死亡済みなら
		if (coli->IsDead())
		{
			//飛ばす
			continue;
		}

		for (int j = i + 1; j < colSize; j++)
		{
			//コライダ
			auto& colj = colliders_[j];

			//コライダが死亡済みなら
			if (coli->IsDead() || colj->IsDead())
			{
				//飛ばす
				continue;
			}

			//同一所持者をスキップ
			if (coli->GetOwner() == colj->GetOwner())
			{
				//飛ばす
				continue;
			}

			//当たり判定前に切り捨て
			if (!IsBroudCollision(*coli->GetOwner(), *colj->GetOwner()))
			{
				//飛ばす
				continue;
			}

			//当たり判定をするタグか
			if (!IsCollisionTag(*coli, *colj))
			{
				//飛ばす
				continue;
			}

			//当たり判定をするか
			if (!JudgeIsCollision(i, j))
			{
				//当たり判定の条件に合わなかったので飛ばす
				continue;
			}

			//当たり判定
			if (IsCollision(*coli, *colj))
			{
				//それぞれの当たった処理
				coli->OnHit(*colj);
				colj->OnHit(*coli);

				//当たった後の処理
				if (!coli->IsDead())
				{
					coli->GetGeometry().HitAfter();
				}
				if (!colj->IsDead())
				{
					colj->GetGeometry().HitAfter();
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

	//**********************************************************
	//ここに当たり判定する範囲の広さをタグごとで設定する
	//**********************************************************

	hitRange_[Collider::TAG::PLAYER1] = HIT_RANGE_NORMAL;
	hitRange_[Collider::TAG::PLAYER2] = HIT_RANGE_NORMAL;
	hitRange_[Collider::TAG::PLAYER3] = HIT_RANGE_NORMAL;
	hitRange_[Collider::TAG::PLAYER4] = HIT_RANGE_NORMAL;
	hitRange_[Collider::TAG::FOOT] = HIT_RANGE_NORMAL;
	hitRange_[Collider::TAG::SPIN] = HIT_RANGE_NORMAL;
	hitRange_[Collider::TAG::MACHINE] = HIT_RANGE_NORMAL;
	hitRange_[Collider::TAG::MACHINE_RIDE] = HIT_RANGE_NORMAL;
	hitRange_[Collider::TAG::NORMAL_OBJECT] = HIT_RANGE_OBJECT;
	hitRange_[Collider::TAG::TREE] = HIT_RANGE_OBJECT;
	hitRange_[Collider::TAG::GLIDE_STAGE] = HIT_RANGE_OBJECT;
	hitRange_[Collider::TAG::GROUND] = HIT_RANGE_GROUND;
	hitRange_[Collider::TAG::ITEM_BOX] = HIT_RANGE_OBJECT;
	hitRange_[Collider::TAG::POWER_UP] = HIT_RANGE_NORMAL;
	hitRange_[Collider::TAG::BATTLE_ITEM] = HIT_RANGE_NORMAL;
	hitRange_[Collider::TAG::CANNON_SHOT] = HIT_RANGE_NORMAL;
	hitRange_[Collider::TAG::SEARCH] = HIT_RANGE_NORMAL;
	hitRange_[Collider::TAG::WORLD_BORDER] = HIT_RANGE_GROUND;
	hitRange_[Collider::TAG::GLIDER_BORDER] = HIT_RANGE_OBJECT;
}

CollisionManager::~CollisionManager(void)
{
}

const bool CollisionManager::IsBroudCollision(const ObjectBase& _obj1, const ObjectBase& _obj2)
{
	//座標(ワールド)
	const VECTOR& pos1 = _obj1.GetTrans().pos;
	const VECTOR& pos2 = _obj2.GetTrans().pos;

	//距離
	float range = _obj1.GetBroudRadius() + _obj2.GetBroudRadius();

	//判定
	return Utility::MagnitudeF(VAdd(pos1, pos2)) < range;
}

const bool CollisionManager::IsCollisionTag(const Collider& _col1, const Collider& _col2) const
{
	//タグ
	const auto& tag1 = _col1.GetTag();
	const auto& tag2 = _col2.GetTag();

	//当たり判定しないタグ
	const auto& notColTags1 = _col1.GetNotHitTags();
	const auto& notColTags2 = _col2.GetNotHitTags();

	//2人目の当たり判定しないタグ
	for (const auto& notColTag2 : notColTags2)
	{
		if (tag1 == notColTag2)
		{
			//1人目のタグが2人目の当たり判定しないタグと同一だった
			return false;
		}
	}

	//1人目の当たり判定しないタグ
	for (const auto& notColTag1 : notColTags1)
	{
		if (tag2 == notColTag1)
		{
			//2人目のタグが1人目の当たり判定しないタグと同一だった
			return false;
		}
	}

	//当たる
	return true;
}

const bool CollisionManager::IsWithInHitRange(const Collider& _col1, const Collider& _col2) const
{
	//双方の距離
	double sqrtDis = Utility::SqrMagnitude(
		_col1.GetGeometry().GetColPos(),
		_col2.GetGeometry().GetColPos());

	//タグ
	const auto& tag1 = _col1.GetTag();
	const auto& tag2 = _col2.GetTag();

	//双方のタグ

	//距離範囲の比較
	auto it1 = hitRange_.find(tag1);
	auto it2 = hitRange_.find(tag2);
	if (it1 == hitRange_.end() || it2 == hitRange_.end()) return false;
	float range = std::max(it1->second, it2->second);

	//当たったなら強制終了
	return sqrtDis <= range * range;
}

const bool CollisionManager::JudgeIsCollision(const int _col1Num, const int _col2Num)const
{
	//コライダ
	auto& col1 = colliders_[_col1Num];
	auto& col2 = colliders_[_col2Num];

	//範囲内か
	if (!IsWithInHitRange(*col1, *col2))
	{
		//範囲内でなかった
		return false;
	}

	//全判定をクリアしたので当たり判定をする
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