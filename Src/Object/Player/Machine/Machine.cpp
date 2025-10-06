#include <DxLib.h>
#include "Machine.h"

Machine::Machine(void)
{
	trans_ = Transform();
}

Machine::~Machine(void)
{
}

void Machine::Load(void)
{
	//ã@ëÃÇÃê´î\ÇÕäOïîÇ©ÇÁéùÇ¡ÇƒÇ≠ÇÈ
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

void Machine::Init(void)
{
}

void Machine::Update(void)
{
}

void Machine::Draw(void)
{
	DrawSphere3D(trans_.pos, 30.0f, 20, 0xff0000, 0xff0000, true);
}
