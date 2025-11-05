#include"../pch.h"
#include "CharacterAction.h"

CharacterAction::CharacterAction(Player& _player, Character& _chara, LogicBase& _logic)
	: ActionBase(_player, _logic),
	chara_(_chara)
{
}

CharacterAction::~CharacterAction(void)
{
}

void CharacterAction::Init(void)
{
	//chara_.GetAnim().Play()
}

void CharacterAction::Update(void)
{

}

void CharacterAction::Draw(void)
{
}
