#include"../pch.h"
#include "../../Manager/Game/CollisionManager.h"
#include "../../Manager/Game/AttackManager.h"
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
	isEnabled_ = true;

	//マネージャーに登録
	CollisionManager::GetInstance().AddCollider(this);
}

Collider::~Collider(void)
{
	//配列削除
	notHitTags_.clear();

	//マネージャーから削除
	CollisionManager::GetInstance().DeleteCollider(this);
	AttackManager::GetInstance().DeleteAttackCollider(this);
}

void Collider::OnHit(const Collider* _collider)
{
	//無効なら何もしない
	if (!isEnabled_)return;

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