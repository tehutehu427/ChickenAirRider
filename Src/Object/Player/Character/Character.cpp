#include "Character.h"

Character::Character(void)
{
}

Character::~Character(void)
{
}

void Character::Load(void)
{
	//キャラクターのステータスを外部から持ってくる
	param_.maxHealth_ = 1;
	param_.maxSpeed_ = 1;
	param_.acceleration_ = 1;
	param_.attack_ = 1;
	param_.defense_ = 1;
	param_.charge_ = 1;
}

void Character::Init(void)
{
}

void Character::Update(void)
{
}

void Character::Draw(void)
{
}
