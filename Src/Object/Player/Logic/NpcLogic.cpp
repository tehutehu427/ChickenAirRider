#include"../pch.h"
#include "NpcLogic.h"

NpcLogic::NpcLogic(void)
{
}

NpcLogic::~NpcLogic(void)
{
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
