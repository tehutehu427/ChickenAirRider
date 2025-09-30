#include <DxLib.h>
#include "../Manager/System/KeyConfig.h"
#include "PlayerLogic.h"

PlayerLogic::PlayerLogic(void)
{
}

PlayerLogic::~PlayerLogic(void)
{
}

const bool PlayerLogic::StartCharge(void)const
{
    return false;
}

const float PlayerLogic::TurnValue(void)const
{
    //インスタンス
    auto& key = KeyConfig::GetInstance();

    //返す値
    float ret = 0.0f;

    //回転量
    ret = GetJoypadNum() <= 0 ? key.GetMouseMove().x : key.GetKnockLStickSize(KeyConfig::JOYPAD_NO::PAD1).x;

    return ret;
}
