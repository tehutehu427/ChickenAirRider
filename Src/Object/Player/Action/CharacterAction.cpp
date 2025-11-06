#include"../pch.h"
#include "../Utility/Utility.h"
#include "../Manager/Game/GravityManager.h"
#include "CharacterAction.h"

CharacterAction::CharacterAction(Player& _player, Character& _chara, LogicBase& _logic)
	: ActionBase(_player, _logic),
	chara_(_chara)
{
	update_[true] = [this](void) {UpdateGround(); };
	update_[false] = [this](void) {UpdateFlight(); };
	walkPow_ = { 0.0f,0.0f };
	jumpPow_ = 0.0f;
}

CharacterAction::~CharacterAction(void)
{
}

void CharacterAction::Init(void)
{
	//立ち状態
	chara_.GetAnim().Play("idle");
}

void CharacterAction::Update(void)
{
	//状態ごとの更新
	update_[player_.IsGrounded()];

	//最終的な移動力の合算
	movePow_ = VGet(walkPow_.x, jumpPow_, walkPow_.y);
	player_.SetMovePow(movePow_);
}

void CharacterAction::Draw(void)
{
}

void CharacterAction::Walk(void)
{
	//歩きの値
	const Vector2F walkValue = logic_.WalkValue();

	//歩いているか
	if (walkValue.x == 0.0f && walkValue.y == 0.0f && player_.IsGrounded())
	{
		//立ち状態
		chara_.GetAnim().Play("idle");
		return;
	}

	//移動量
	walkPow_.x = walkValue.x;
	walkPow_.y = walkValue.y;

	//歩きアニメーション
	chara_.GetAnim().Play("walk");
}

void CharacterAction::Jump(void)
{
	//初動ジャンプ力
	jumpPow_ = JUMP_POW;

	//空中に
	player_.SetIsGrounded(false);
}

void CharacterAction::CalcGravity(void)
{
	//重力
	VECTOR gravPow;
	GravityManager::GetInstance().CalcGravity(Utility::DIR_D, gravPow);

	//ジャンプ力に加算
	jumpPow_ += gravPow.y;
}

void CharacterAction::UpdateGround(void)
{
	//歩き
	Walk();

	//地上でプッシュボタン
	if (logic_.IsPush())
	{
		//ジャンプ
		Jump();
	}
}

void CharacterAction::UpdateFlight(void)
{
	//歩き
	Walk();

	//重力
	CalcGravity();

	//ジャンプアニメーション
	chara_.GetAnim().Play("jump");
}
