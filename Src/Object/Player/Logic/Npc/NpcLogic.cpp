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
    brain_ = std::make_unique<NpcBrain>();
    brain_->Init();
}

void NpcLogic::Update(void)
{
    //çXêV
    brain_->Update();
}

const bool NpcLogic::IsPush(void) 
{
    return false;
}

const bool NpcLogic::StartCharge(void)
{
    return false;
}

const bool NpcLogic::DisCharge(void) 
{
    return false;
}

const Vector2F NpcLogic::TurnValue(void)
{
    return {0.0f,0.0f};
}

const bool NpcLogic::IsSpecial(void)
{
    return false;
}

const bool NpcLogic::IsGetOff(void)
{
    return false;
}

const bool NpcLogic::IsButtonMeshing(void)
{
    return false;
}

const Vector2F NpcLogic::WalkValue(void)
{
    return Vector2F();
}

const bool NpcLogic::IsJump(void)
{
    return false;
}
