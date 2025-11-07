#include"../pch.h"
#include "../Utility/Utility.h"
#include "../Manager/System/Camera.h"
#include "../Manager/Game/GravityManager.h"
#include "CharacterAction.h"

CharacterAction::CharacterAction(Player& _player, Character& _chara, LogicBase& _logic)
	: ActionBase(_player, _logic),
	chara_(_chara)
{
	update_[true] = [this](void) {UpdateGround(); };
	update_[false] = [this](void) {UpdateFlight(); };
	walkPow_ = Utility::VECTOR_ZERO;
	jumpPow_ = Utility::VECTOR_ZERO;
	rot_ = Quaternion();
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
	update_[player_.IsGrounded()]();

	//最終的な移動力の合算
	movePow_ = walkPow_;
	movePow_ = VAdd(movePow_, jumpPow_);

	//移動量
	player_.SetMovePow(movePow_);
}

void CharacterAction::Draw(void)
{
}

void CharacterAction::Walk(void)
{
	//歩きの値
	const Vector2F walkValue = logic_.WalkValue();

	//地上判定
	const bool isGrounded = player_.IsGrounded();

	//移動量
	walkPow_.x = walkValue.x * WALK_POW;
	walkPow_.z = walkValue.y * WALK_POW;

	//立ち止まっているか
	if (walkValue.x == 0.0f && walkValue.y == 0.0f && isGrounded)
	{
		//立ち状態
		chara_.GetAnim().Play("idle");
	}
	//歩いている
	else if(isGrounded)
	{
		//歩きアニメーション
		chara_.GetAnim().Play("walk");
	}

	//カメラ回転
	const auto& camera = player_.GetCamera().lock();
	VECTOR cameraVec = Utility::GetMoveVec(camera->GetPos(), camera->GetTargetPos());
	cameraVec.y = 0.0f;
	Quaternion cameraQuaRot = Quaternion::LookRotation(cameraVec);

	//移動量
	walkPow_ = cameraQuaRot.PosAxis(walkPow_);

	//動いているなら
	if (walkValue.x != 0.0f || walkValue.y != 0.0f)
	{
		//動いた方向に向く
		player_.SetQuaRot(Quaternion::LookRotation(walkPow_));
	}
}

void CharacterAction::Jump(void)
{
	//初動ジャンプ力
	jumpPow_.y = JUMP_POW;

	//空中に
	player_.SetIsGrounded(false);
}

void CharacterAction::CalcGravity(void)
{
	//重力
	VECTOR gravPow;

	//加算
	GravityManager::GetInstance().CalcGravity(Utility::DIR_D, gravPow);
	jumpPow_ = VAdd(jumpPow_, gravPow);
}

void CharacterAction::UpdateGround(void)
{
	//歩き
	Walk();

	//地上でプッシュボタン
	if (logic_.IsJump())
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
	chara_.GetAnim().Play("jump", false);
}
