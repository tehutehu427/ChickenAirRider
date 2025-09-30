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
	param_.maxSpeed_ = 1;
	param_.acceleration_ = 1;
	param_.turning_ = 1;
	param_.charge_ = 1;
	param_.flight_ = 1;
	param_.weight_ = 1;
	param_.attack_ = 1;
	param_.defense_ = 1;
	param_.maxHealth_ = 1;
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
