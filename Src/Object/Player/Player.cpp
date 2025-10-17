#include"../pch.h"
#include "../Utility/Utility.h"
#include "../Manager/Game/GravityManager.h"
#include "../Common/Geometry/Sphere.h"
#include "../Common/Geometry/Line.h"
#include "Character/Character.h"
#include "Machine/Machine.h"
#include "Logic/LogicBase.h"
#include "Logic/PlayerLogic.h"
#include "Logic/NpcLogic.h"
#include "Action/ActionBase.h"
#include "Action/CharacterAction.h"
#include "Action/MachineAction.h"
#include "Player.h"

Player::Player(void)
{
	//初期化
	state_ = STATE::NONE;
	movedPos_ = Utility::VECTOR_ZERO;
	movePow_ = Utility::VECTOR_ZERO;
	isGrounded_ = false;

	//行動切り替え
	changeAction_[STATE::NONE] = [this](void) {};
	changeAction_[STATE::NORMAL] = [this](void) {ChangeActionNormal(); };
	changeAction_[STATE::RIDE_MACHINE] = [this](void) {ChangeActionRide(); };

	//描画
	draw_[STATE::NONE] = [this](void) {};
	draw_[STATE::NORMAL] = [this](void) {DrawNormal(); };
	draw_[STATE::RIDE_MACHINE] = [this](void) {DrawRide(); };
}

Player::~Player(void)
{
}

void Player::Load(void)
{
}

void Player::Init(void)
{
	//キャラクター
	chara_ = std::make_unique<Character>();
	chara_->Load();
	chara_->Init();

	//機体
	machine_ = std::make_unique<Machine>();
	machine_->Load();
	machine_->Init();

	//行動基準
	logic_ = std::make_unique<PlayerLogic>();

	//初期状態
	ChangeState(STATE::RIDE_MACHINE);

	//行動
	changeAction_[state_]();

	//当たり判定生成
	std::unique_ptr<Geometry> geo = std::make_unique<Sphere>(trans_.pos, RADIUS);
	MakeCollider({ Collider::TAG::PLAYER1 }, std::move(geo));

	//接地判定用の当たり判定
	geo = std::make_unique<Line>(trans_.pos, trans_.quaRot, LOCAL_LINE_UP, LOCAL_LINE_DOWN);
	MakeCollider({ Collider::TAG::PLAYER1 }, std::move(geo));
}

void Player::Update(void)
{
	//インスタンス
	auto& grvMng = GravityManager::GetInstance();

	//移動更新
	trans_.pos = movedPos_;

	//行動
	action_->Update();

	//移動
	movedPos_ = VAdd(movedPos_, movePow_);

	trans_.pos = movedPos_;

	VECTOR grav = Utility::VECTOR_ZERO;
	grvMng.CalcGravity(Utility::DIR_D, grav);
	movedPos_ = VAdd(movedPos_, grav);

	isGrounded_ = false;

	//機体とキャラに座標と回転を同期させる
	SynchronizeChara();
	SynchronizeMachine();
}

void Player::Draw(void)
{
	//描画
	draw_[state_]();

	for (auto& col : colParam_)
	{
		col.geometry_->Draw(0);
	}

	DrawFormatString(0, 32, 0xffffff, L"%.2f,%.2f,%.2f", trans_.quaRot.ToEuler().x, trans_.quaRot.ToEuler().y, trans_.quaRot.ToEuler().z);
	DrawFormatString(0, 80, 0xffffff, L"%.2f,%.2f,%.2f", trans_.pos.x, trans_.pos.y, trans_.pos.z);
}

void Player::OnHit(const std::weak_ptr<Collider> _hitCol)
{
	//タグ
	auto& hitTags = _hitCol.lock()->GetTags();

	//コライダ
	auto& groundCol = colParam_[static_cast<int>(COL_VALUE::GROUNDED)].collider_;

	//ステージとぶつかった
	if (std::find(hitTags.begin(), hitTags.end(), Collider::TAG::STAGE) != hitTags.end())
	{
		//位置の補正
		movedPos_ = trans_.pos;

		//接地しているか
		if (groundCol->IsHit())isGrounded_ = true;
	}
}

const Parameter Player::GetAllParam(void)const
{
	//総合パラメーター
	return param_.CalcUnitParam(GetUnitParam());
}

UnitParameter Player::GetUnitParam(void) const
{
	//キャラと機体の合計パラメーター
 	return chara_->GetUnitParam().Calculate(machine_->GetUnitParam());
}

void Player::ChangeActionNormal(void)
{
	//キャラクターの行動に変更
	action_ = std::make_unique<CharacterAction>(*this, *chara_, *logic_);
}

void Player::ChangeActionRide(void)
{
	//機体の行動に変更
	action_ = std::make_unique<MachineAction>(*this, *machine_, *logic_);
}

void Player::DrawNormal(void)
{
	//キャラの描画
	chara_->Draw();
}

void Player::DrawRide(void)
{
	//キャラの描画
	chara_->Draw();

	//機体の描画
	machine_->Draw();

	//UI
	action_->Draw();
}

void Player::SynchronizeChara(void)
{
	//座標と回転の同期
	chara_->SetPos(trans_.pos);
	chara_->SetQuaRot(trans_.quaRot);
	chara_->Update();
}

void Player::SynchronizeMachine(void)
{
	//座標と回転の同期
	machine_->SetPos(trans_.pos);
	machine_->SetQuaRot(trans_.quaRot);
	machine_->Update();
}
