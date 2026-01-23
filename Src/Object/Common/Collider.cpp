#include"../pch.h"
#include "../ObjectBase.h"
#include "Geometry/Geometry.h"
#include "Collider.h"

Collider::Collider(ObjectBase* _parent, const TAG _tag, std::unique_ptr<Geometry> _geometry, const std::set<TAG> _notHitTags) :
	owner_(_parent),
	myTag_(_tag),
	geometry_(std::move(_geometry)),
	notHitTags_(_notHitTags)
{
	isHit_ = false;
	isDead_ = false;
}

Collider::~Collider(void)
{
	notHitTags_.clear();
	owner_ = nullptr;
}

void Collider::OnHit(const Collider& _collider)
{
	//この当たり判定が当たった
	isHit_ = true;

	//親に相手のコライダを渡す
	owner_->OnHit(_collider);

	//当たり判定が終わった
	isHit_ = false;
}
