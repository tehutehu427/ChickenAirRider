#include"../pch.h"
#include "Character.h"

Character::Character(void)
{
	trans_ = Transform();
}

Character::~Character(void)
{
}

void Character::Load(void)
{
	//キャラクターのステータスを外部から持ってくる
	unitParam_.fixedMaxSpeed_ = PARAM_NORMAL;
	unitParam_.fixedAcceleration_ = PARAM_NORMAL;
	unitParam_.fixedTurning_ = PARAM_NORMAL;
	unitParam_.fixedCharge_ = PARAM_NORMAL;
	unitParam_.fixedFlight_ = PARAM_NORMAL;
	unitParam_.fixedWeight_ = PARAM_NORMAL;
	unitParam_.fixedAttack_ = PARAM_NORMAL;
	unitParam_.fixedDefence_ = PARAM_NORMAL;
	unitParam_.fixedMaxHealth_ = PARAM_NORMAL;

	unitParam_.affectMaxSpeed_ = 0.1f;
	unitParam_.affectAcceleration_ = 0.1f;
	unitParam_.affectTurning_ = 0.1f;
	unitParam_.affectCharge_ = 0.1f;
	unitParam_.affectFlight_ = 0.1f;
	unitParam_.affectWeight_ = 0.1f;
	unitParam_.affectAttack_ = 0.1f;
	unitParam_.affectDefence_ = 0.1f;
	unitParam_.affectMaxHealth_ = 0.1f;

	unitParam_.chargeBraking_ = 0.2f;
	unitParam_.chargeCapacity_ = 100.0f;
}

void Character::Init(void)
{
}

void Character::Update(void)
{
}

void Character::Draw(void)
{
	DrawSphere3D(trans_.pos, 10.0f, 20, 0xffff00, 0xffff00, true);
}
