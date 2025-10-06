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
	unitParam_.affectMaxSpeed_ = PARAM_NORMAL;
	unitParam_.affectAcceleration_ = PARAM_NORMAL;
	unitParam_.affectTurning_ = PARAM_NORMAL;
	unitParam_.affectCharge_ = PARAM_NORMAL;
	unitParam_.affectFlight_ = PARAM_NORMAL;
	unitParam_.affectWeight_ = PARAM_NORMAL;
	unitParam_.affectAttack_ = PARAM_NORMAL;
	unitParam_.affectDefence_ = PARAM_NORMAL;
	unitParam_.affectMaxHealth_ = PARAM_NORMAL;

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
