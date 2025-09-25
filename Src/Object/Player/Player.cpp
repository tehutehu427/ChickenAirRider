#include "Character/Character.h"
#include "Machine/Machine.h"
#include "Logic/LogicBase.h"
#include "Logic/PlayerLogic.h"
#include "Logic/NpcLogic.h"
#include "Action/ActionBase.h"
#include "Action/CharacterAction.h"
#include "Action/MachineAction.h"
#include "Player.h"

Player::Player(void)
{
}

Player::~Player(void)
{
}

void Player::Load(void)
{
}

void Player::Init(void)
{
	//キャラクター
	chara_ = std::make_unique<Character>();
	chara_->Init();

	//機体
	machine_ = std::make_unique<Machine>();
	machine_->Init();

	//行動基準
	logic_ = std::make_unique<PlayerLogic>();

	//初期状態
}

void Player::Update(void)
{
}

void Player::Draw(void)
{
}

void Player::OnHit(const std::weak_ptr<Collider> _hitCol)
{
}
