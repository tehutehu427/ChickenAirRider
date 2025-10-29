#include "../pch.h"
#include "../Player.h"
#include "../Action/ActionBase.h"
#include "PlayerOnHit.h"

PlayerOnHit::PlayerOnHit(Player& _player)
	: player_(_player)
{
	//タグごとのヒット処理格納
	onHit_[Collider::TAG::NORMAL_OBJECT] = [this](const std::weak_ptr<Collider> _hitCol) {NormalObjectOnHit(_hitCol); };
	onHit_[Collider::TAG::GROUND] = [this](const std::weak_ptr<Collider> _hitCol) {NormalObjectOnHit(_hitCol); };
}

PlayerOnHit::~PlayerOnHit(void)
{
}

void PlayerOnHit::OnHit(const std::weak_ptr<Collider> _hitCol)
{
	//タグ
	const auto& hitTags = _hitCol.lock()->GetTags();

	for (const auto& hitTag : hitTags)
	{
		//タグごとのヒット処理
		onHit_[hitTag](_hitCol);
	}
}

void PlayerOnHit::NormalObjectOnHit(const std::weak_ptr<Collider> _hitCol)
{
	//コライダ
	auto& groundCol = player_.colParam_[static_cast<int>(Player::COL_VALUE::GROUNDED)].collider_;

	//位置の補正
	player_.trans_.pos = VSub(player_.trans_.pos, VScale(_hitCol.lock()->GetGeometry().GetHitNormal(), _hitCol.lock()->GetGeometry().GetHitDepth()));

	//接地しているか
	if (groundCol->IsHit())
	{
		player_.action_->ResetAxisX();
		player_.isGrounded_ = true;
	}
}
