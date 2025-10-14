#include"../pch.h"
#include "../Application.h"
#include "../Manager/System/SceneManager.h"
#include "../Manager/System/KeyConfig.h"
#include "PlayerLogic.h"

PlayerLogic::PlayerLogic(void)
{
}

PlayerLogic::~PlayerLogic(void)
{
}

const bool PlayerLogic::IsPush(void) 
{
    return false;
}

const bool PlayerLogic::StartCharge(void)
{
    //インスタンス
    auto& key = KeyConfig::GetInstance();

    //チャージ開始ボタンを押しているか
    if (key.IsNew(KeyConfig::CONTROL_TYPE::PUSH_BUTTON, KeyConfig::JOYPAD_NO::PAD1))
    {
        //押している
        return true;
    }

    //押していない
    return false;
}

const bool PlayerLogic::DisCharge(void) 
{
    //インスタンス
    auto& key = KeyConfig::GetInstance();

    //チャージ開始ボタンを離したか
    if (key.IsTrgUp(KeyConfig::CONTROL_TYPE::PUSH_BUTTON, KeyConfig::JOYPAD_NO::PAD1))
    {
        //離した
        return true;
    }

    //離していない
    return false;
}

const float PlayerLogic::TurnValue(void)
{
    //インスタンス
    auto& key = KeyConfig::GetInstance();

    //返す値
    float ret = 0.0f;

    //キーボード又はスティックの移動量
    if (key.IsNew(KeyConfig::CONTROL_TYPE::MACHINE_TURN_LEFT, KeyConfig::JOYPAD_NO::PAD1))
    {
        ret = -TURN_POW;
    }
    else if (key.IsNew(KeyConfig::CONTROL_TYPE::MACHINE_TURN_RIGHT, KeyConfig::JOYPAD_NO::PAD1))
    {
        ret = TURN_POW;
    }

    //ゲームコントローラーがついてないなら
    if(GetJoypadNum() < 1)key.SetMousePos({ Application::SCREEN_HALF_X, Application::SCREEN_HALF_Y });

    return ret;
}
