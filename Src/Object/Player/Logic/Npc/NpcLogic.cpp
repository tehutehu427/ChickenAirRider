#include"../pch.h"
#include "NpcBrain.h"
#include "NpcLogic.h"

NpcLogic::NpcLogic(const Player& _parent)
    : parent_(_parent)
{
}

NpcLogic::~NpcLogic(void)
{
}

void NpcLogic::Init(void)
{
    //çsìÆäÓèÄ
    brain_ = std::make_unique<NpcBrain>(parent_, *this);
    brain_->Init();
}

void NpcLogic::Update(void)
{
    //çXêV
    brain_->Update();
}

const bool NpcLogic::IsPush(void) 
{
    return push_;
}

const bool NpcLogic::StartCharge(void)
{
    return startCharge_;
}

const bool NpcLogic::DisCharge(void) 
{
    return disCharge_;
}

const Vector2F NpcLogic::TurnValue(void)
{
    return turnValue_;
}

const bool NpcLogic::IsSpecial(void)
{
    return special_;
}

const bool NpcLogic::IsGetOff(void)
{
    return getOff_;
}

const bool NpcLogic::IsButtonMeshing(void)
{
    return buttonMeshing_;
}

const Vector2F NpcLogic::WalkValue(void)
{
    return walkValue_;
}

const bool NpcLogic::IsJump(void)
{
    return jump_;
}
