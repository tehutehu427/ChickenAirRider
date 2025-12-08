#include "../pch.h"
#include "../Player.h"
#include "../Action/ActionBase.h"
#include "PlayerUI.h"

PlayerUI::PlayerUI(Player& _player) :
	player_(_player)
{
}

PlayerUI::~PlayerUI(void)
{
}

void PlayerUI::Draw(void)
{
	//ƒAƒNƒVƒ‡ƒ“UI
	player_.GetAction().Draw();
}
