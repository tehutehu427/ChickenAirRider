#include"../pch.h"
#include "../Application.h"
#include "../Manager/System/SceneManager.h"
#include "../Manager/System/KeyConfig.h"
#include "../Common/Vector2F.h"
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
    if (key.IsTrgHold(KeyConfig::CONTROL_TYPE::PUSH_BUTTON, KeyConfig::JOYPAD_NO::PAD1, CHARGE_START_PUSH_TIME))
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
    if (key.IsTrgUp(KeyConfig::CONTROL_TYPE::PUSH_BUTTON, KeyConfig::JOYPAD_NO::PAD1) &&
        key.IsTrgHold(KeyConfig::CONTROL_TYPE::PUSH_BUTTON, KeyConfig::JOYPAD_NO::PAD1, CHARGE_START_PUSH_TIME))
    {
        //離した
        return true;
    }

    //離していない
    return false;
}

const Vector2F PlayerLogic::TurnValue(void)
{
    //インスタンス
    auto& key = KeyConfig::GetInstance();

    //返す値
    Vector2F ret = {0.0f,0.0f};

    //キーボード又はスティックの移動量
    if (key.IsNew(KeyConfig::CONTROL_TYPE::MACHINE_TURN_LEFT, KeyConfig::JOYPAD_NO::PAD1))
    {
        //左方向なのでマイナス
        ret.x = -TURN_POW;
    }
    else if (key.IsNew(KeyConfig::CONTROL_TYPE::MACHINE_TURN_RIGHT, KeyConfig::JOYPAD_NO::PAD1))
    {
        //右方向なのでプラス
        ret.x = TURN_POW;
    }

    if (key.IsNew(KeyConfig::CONTROL_TYPE::MACHINE_TURN_FRONT, KeyConfig::JOYPAD_NO::PAD1))
    {
        //前方向なのでプラス
        ret.y = TURN_POW;
    }
    else if (key.IsNew(KeyConfig::CONTROL_TYPE::MACHINE_TURN_BACK, KeyConfig::JOYPAD_NO::PAD1))
    {
        //後方向なのでマイナス
        ret.y = -TURN_POW;
    }

    //ゲームコントローラーがついてないなら
    if(GetJoypadNum() < 1)key.SetMousePos({ Application::SCREEN_HALF_X, Application::SCREEN_HALF_Y });

    return ret;
}
