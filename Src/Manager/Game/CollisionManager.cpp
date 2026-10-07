#include"../pch.h"
#include"../Object/Common/Transform.h"
#include"../Object/Common/Geometry/Geometry.h"
#include"../Object/Common/Collider.h"
#include"../Object/ObjectBase.h"
#include"../Utility/Utility.h"
#include "CollisionManager.h"

void CollisionManager::AddCollider(Collider* _collider)
{
	//コライダの追加
	colliders_.push_back(_collider);
}

void CollisionManager::DeleteCollider(Collider* _collider)
{
	//コライダの削除
	colliders_.erase(std::remove(colliders_.begin(), colliders_.end(), _collider), colliders_.end());
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
		//コライダが無効なら
		if (!colliders_[i]->IsEnabled())
		{
			//飛ばす
			continue;
		}

		for (int j = i + 1; j < colSize; j++)
		{
			//コライダが無効なら
			if (!colliders_[i]->IsEnabled() || !colliders_[j]->IsEnabled())
			{
				//飛ばす
				continue;
			}

			//事前当たり判定
			if (!PreCollision(colliders_[i], colliders_[j]))
			{
				//飛ばす
				continue;
			}

			//当たり判定をするタグか
			if (!CheckCollisionTags(colliders_[i], colliders_[j]))
			{
				//飛ばす
				continue;
			}

			//当たり判定
			CollisionGeometry(colliders_[i], colliders_[j]);
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

void CollisionManager::CollisionGeometry(Collider* _col1, Collider* _col2)
{
	//コライダ
	const auto& col1 = _col1;
	const auto& col2 = _col2;

	//形状同士の当たり判定
	auto& geo1 = col1->GetGeometry();
	auto& geo2 = col2->GetGeometry();

	//衝突があった場合
	if (geo1.IsHit(geo2)) {

		//各ヒット処理
		col1->OnHit(_col2);
		col2->OnHit(_col1);

		//ヒット後処理
		col1->GetGeometry().HitAfter();
		col2->GetGeometry().HitAfter();
	}
}

const bool CollisionManager::CheckCollisionTags(const Collider* _col1, const Collider* _col2)const
{
	//タグの確認
	const auto& type1 = _col1->GetTag();
	const auto& type2 = _col2->GetTag();

	//タグが一致するものがあったら、当たり判定を行わない
	for (auto& type : _col2->GetNotHitTags()) {
		if (type == type1) {
			return false;
		}
	}

	//タグが一致するものがあったら、当たり判定を行わない
	for (auto& type : _col1->GetNotHitTags()) {
		if (type == type2) {
			return false;
		}
	}

	return true;
}

const bool CollisionManager::PreCollision(Collider* _col1, Collider* _col2)const
{
	//形状
	const auto& geo1 = _col1->GetGeometry();
	const auto& geo2 = _col2->GetGeometry();

	//原点
	const VECTOR& origin = geo1.GetColPos();

	//座標
	const VECTOR& pos1 = Utility::VECTOR_ZERO;
	const VECTOR& pos2 = VSub(geo2.GetColPos(), origin);

	//距離
	float range = geo1.GetBroudRadius() + geo2.GetBroudRadius();
	float sqrDistance = Utility::SqrMagnitudeF(VAdd(pos1, pos2));

	//判定
	return sqrDistance < range * range;
}
