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
	//‹@‘Ì‚Ì«”\‚ÍŠO•”‚©‚ç‚Á‚Ä‚­‚é
	param_.maxSpeed_ = 10;
	param_.acceleration_ = 1;
	param_.turning_ = 10;
	param_.charge_ = 10;
	param_.flight_ = 1;
	param_.weight_ = 1;
	param_.attack_ = 1;
	param_.defense_ = 1;
	param_.maxHealth_ = 1;
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
