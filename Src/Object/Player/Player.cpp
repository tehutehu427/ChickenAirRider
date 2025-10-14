#include"../pch.h"
#include "../Utility/Utility.h"
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
	movePow_ = Utility::VECTOR_ZERO;

	//行動切り替え
	changeAction_[STATE::NONE] = [this](void)
	{
	};
	changeAction_[STATE::NORMAL] = [this](void)
	{
		//キャラクターの行動に変更
		action_ = std::make_unique<CharacterAction>(*this, *chara_, *logic_);
	};
	changeAction_[STATE::RIDE_MACHINE] = [this](void)
	{
		//機体の行動に変更
		action_ = std::make_unique<MachineAction>(*this, *machine_, *logic_);
	};

	//描画
	draw_[STATE::NONE] = [this](void)
	{
	};
	draw_[STATE::NORMAL] = [this](void)
	{
		//キャラの描画
		chara_->Draw();
	};
	draw_[STATE::RIDE_MACHINE] = [this](void)
	{
		//キャラの描画
		chara_->Draw();

		//機体の描画
		machine_->Draw();

		//UI
		action_->Draw();
	};
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
}

void Player::Update(void)
{
	//行動
	action_->Update();

	//移動
	trans_.pos = VAdd(trans_.pos, movePow_);

	//機体とキャラに座標と回転を同期させる
	SynchronizeChara();
	SynchronizeMachine();
}

void Player::Draw(void)
{
	//描画
	draw_[state_]();

	DrawFormatString(0, 32, 0xffffff, L"%.2f,%.2f,%.2f", trans_.quaRot.ToEuler().x, trans_.quaRot.ToEuler().y, trans_.quaRot.ToEuler().z);
}

void Player::OnHit(const std::weak_ptr<Collider> _hitCol)
{
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

void Player::SynchronizeChara(void)
{
	//座標と回転の同期
	chara_->SetPos(trans_.pos);
	chara_->SetQuaRot(trans_.quaRot);
}

void Player::SynchronizeMachine(void)
{
	//座標と回転の同期
	machine_->SetPos(trans_.pos);
	machine_->SetQuaRot(trans_.quaRot);
}
