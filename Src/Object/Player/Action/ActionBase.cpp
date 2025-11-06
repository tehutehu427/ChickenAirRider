#include"../pch.h"
#include "../Player.h"
#include "../Utility/Utility.h"
#include "ActionBase.h"

ActionBase::ActionBase(Player& _player, LogicBase& _logic)
	: player_(_player),
	logic_(_logic)
{
	movePow_ = Utility::VECTOR_ZERO;
	axis_ = Utility::VECTOR_ZERO;
}

ActionBase::~ActionBase(void)
{
}

void ActionBase::Init(void)
{
}

void ActionBase::Update(void)
{
}

void ActionBase::Draw(void)
{
}

void ActionBase::UpdateGround(void)
{
}

void ActionBase::UpdateFlight(void)
{
}
