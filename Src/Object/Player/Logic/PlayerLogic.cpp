#include"../pch.h"
#include "../Application.h"
#include "../Manager/System/SceneManager.h"
#include "../Manager/System/KeyConfig.h"
#include "../Common/Vector2F.h"
#include "PlayerLogic.h"

PlayerLogic::PlayerLogic(void)
{
    buttonMeshingCnt_ = 0;
    cnt_ = 0.0f;
    oldTurnValue_ = {};
    newTurnValue_ = {};
}

PlayerLogic::~PlayerLogic(void)
{
}

const bool PlayerLogic::IsPush(void) 
{
    //インスタンス
    auto& key = KeyConfig::GetInstance();

    //プッシュボタンを押しているか
    if (key.IsNew(KeyConfig::CONTROL_TYPE::PUSH_BUTTON, KeyConfig::JOYPAD_NO::PAD1))
    {
        //押された
        return true;
    }

    return false;
}

const bool PlayerLogic::StartCharge(void)
{
    //インスタンス
    auto& key = KeyConfig::GetInstance();

    //プッシュボタンを押しているか
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

const bool PlayerLogic::IsSpecial(void)
{
    //インスタンス
    auto& key = KeyConfig::GetInstance();

    //スペシャルボタンを押したか
    if (key.IsTrgUp(KeyConfig::CONTROL_TYPE::SPECIAL_BUTTON, KeyConfig::JOYPAD_NO::PAD1) &&
        !IsGetOff())
    {
        //押した
        return true;
    }

    //押していない
    return false;
}

const bool PlayerLogic::IsGetOff(void)
{
    //インスタンス
    auto& key = KeyConfig::GetInstance();

    //スペシャルボタンを一定時間押し続けているか
    if (key.IsTrgHold(KeyConfig::CONTROL_TYPE::SPECIAL_BUTTON, KeyConfig::JOYPAD_NO::PAD1, GETOFF_PUSH_TIME, true))
    {
        //押し続けた
        return true;
    }

    //押していない又は離した
    return false;
}

const bool PlayerLogic::IsButtonMeshing(void)
{
    //インスタンス
    const auto& key = KeyConfig::GetInstance();

    //デルタタイム
    const auto& delta = SceneManager::GetInstance().GetDeltaTime();

    //傾けた量
    Vector2F turn = TurnValue();
    oldTurnValue_ = newTurnValue_;
    if (turn.x != 0.0f)
    {
        newTurnValue_ = turn;
    }

    //前フレームと比べて+-値が反対なら
    if (oldTurnValue_.x <= 0.0f && newTurnValue_.x > 0.0f
        || oldTurnValue_.x >= 0.0f && newTurnValue_.x < 0.0f)
    {
        //レバガチャ判定
        buttonMeshingCnt_++;

        //受付時間カウンタリセット
        cnt_ = 0.0f;
    }
    else
    {
        //カウント
        cnt_ += delta;
    }

    //レバガチャ判定
    if (buttonMeshingCnt_ >= BUTTON_MESHING_MAX)
    {
        //リセット
        buttonMeshingCnt_ = 0;

        //レバガチャした
        return true;
    }

    //時間制限
    if (cnt_ > BUTTON_MESHING_RIMIT)
    {
        //リセット
        buttonMeshingCnt_ = 0;
    }

    return false;
}

const Vector2F PlayerLogic::WalkValue(void)
{
    //インスタンス
    auto& key = KeyConfig::GetInstance();

    //返す値
    Vector2F movePow = { 0.0f,0.0f };

    const float stickPow = 1000.0f;

    if (GetJoypadNum() < 1)
    {
        //キーボード又はスティックの移動量
        if (key.IsNew(KeyConfig::CONTROL_TYPE::CHARACTER_MOVE_LEFT, KeyConfig::JOYPAD_NO::PAD1))
        {
            //左方向なのでマイナス
            movePow.x = -MOVE_POW;
        }
        else if (key.IsNew(KeyConfig::CONTROL_TYPE::CHARACTER_MOVE_RIGHT, KeyConfig::JOYPAD_NO::PAD1))
        {
            //右方向なのでプラス
            movePow.x = MOVE_POW;
        }

        if (key.IsNew(KeyConfig::CONTROL_TYPE::CHARACTER_MOVE_FRONT, KeyConfig::JOYPAD_NO::PAD1))
        {
            //前方向なのでプラス
            movePow.y = MOVE_POW;
        }
        else if (key.IsNew(KeyConfig::CONTROL_TYPE::CHARACTER_MOVE_BACK, KeyConfig::JOYPAD_NO::PAD1))
        {
            //後方向なのでマイナス
            movePow.y = -MOVE_POW;
        }
    }
    else
    {
        //スティック
        Vector2F stick = key.GetKnockLStickSize(KeyConfig::JOYPAD_NO::PAD1).ToVector2F() / stickPow;

        //スティックの傾き
        movePow.x = stick.x;
        movePow.y = -stick.y;
    }

    return movePow;
}

const bool PlayerLogic::IsJump(void)
{
    //インスタンス
    auto& key = KeyConfig::GetInstance();

    //ジャンプが押されたか
    if (key.IsTrgDown(KeyConfig::CONTROL_TYPE::PUSH_BUTTON, KeyConfig::JOYPAD_NO::PAD1))
    {
        //押された
        return true;
    }

    //押されなかった
    return false;
}
