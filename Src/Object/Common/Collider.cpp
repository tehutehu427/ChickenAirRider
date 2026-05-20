#include"../pch.h"
#include "../ObjectBase.h"
#include "Geometry/Geometry.h"
#include "Collider.h"

Collider::Collider(ObjectBase& _owner, const TAG _tag, std::unique_ptr<Geometry> _geometry, const std::set<TAG> _notHitTags) :
	owner_(_owner),
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
}

void Collider::Kill(void)
{
	isDead_ = true;
}

void Collider::OnHit(const std::weak_ptr<Collider> _collider)
{
	//死亡済み
	if (isDead_)return;

	//この当たり判定が当たった
	isHit_ = true;

	//親に相手のコライダを渡す
	owner_.OnHit(_collider);

	//当たり判定が終わった
	isHit_ = false;
}

const bool Collider::IsIncludeMyTag(const std::set<TAG>& _tags) const
{
	//指定されたタグのどれかが自身のタグに含まれているか
	return _tags.contains(myTag_);
}

const bool Collider::IsIncludeNotHitTag(const std::set<TAG>& _tags) const
{
	//衝突させないタグのどれかが指定されたタグに含まれているか
	return std::any_of(_tags.begin(), _tags.end(), [this](const TAG& tag) { return notHitTags_.contains(tag); });
}