#include "../Player.h"
#include "ActionBase.h"

ActionBase::ActionBase(Player& _player, const LogicBase& _logic)
	: player_(_player),
	logic_(_logic)
{
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