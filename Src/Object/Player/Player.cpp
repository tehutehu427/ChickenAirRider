#include "../Utility/Utility.h"
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
	movePow_ = Utility::VECTOR_ZERO;
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
}

void Player::Update(void)
{
	//移動
	trans_.pos = VAdd(trans_.pos, movePow_);
}

void Player::Draw(void)
{
}

void Player::OnHit(const std::weak_ptr<Collider> _hitCol)
{
}

const Parameter& Player::GetAllParam(void)const
{
	//総合パラメーター
	Parameter ret;

	//取得した強化アイテム + キャラの基本性能 + 機体の基本性能
	ret = param_ + chara_->GetParam() + machine_->GetParam();

	return ret;
}
