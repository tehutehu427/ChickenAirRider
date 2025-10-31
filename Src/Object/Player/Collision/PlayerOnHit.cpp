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
	const auto& hitGeo = _hitCol.lock()->GetGeometry();

	//コライダ
	auto& mainCol = player_.collider_[static_cast<int>(Player::COL_VALUE::MAIN)];
	auto& groundCol = player_.collider_[static_cast<int>(Player::COL_VALUE::GROUNDED)];

	//位置の補正
	const auto& hit = mainCol->GetGeometry().GetHitResult();

	VECTOR v = VSub(player_.movedPos_, player_.trans_.pos);

	// 衝突時位置まで移動
	player_.movedPos_ = VAdd(player_.trans_.pos, VScale(v, hit.t));

	// 少し押し戻す（めり込み回避）
	player_.movedPos_ = VAdd(player_.movedPos_, VScale(hit.normal, 0.01f));

	// 残り移動をスライド方向へ
	float remain = 1.0f - hit.t;
	VECTOR slide = VSub(v, VScale(hit.normal, VDot(v, hit.normal)));
	player_.movedPos_ = VAdd(player_.movedPos_, VScale(slide, remain));

	//接地しているか
	if (groundCol->IsHit())
	{
		player_.action_->ResetAxisX();
		player_.isGrounded_ = true;
	}
}
