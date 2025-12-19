#include"../pch.h"
#include "UnitParameter.h"

UnitParameter UnitParameter::Calculate(const UnitParameter& _unitParam) const
{
    UnitParameter ret;

    //固定値
    ret.fixedMaxSpeed = fixedMaxSpeed + _unitParam.fixedMaxSpeed;
    ret.fixedAcceleration = fixedAcceleration + _unitParam.fixedAcceleration;
    ret.fixedTurning = fixedTurning + _unitParam.fixedTurning;
    ret.fixedCharge = fixedCharge + _unitParam.fixedCharge;
    ret.fixedFlight = fixedFlight + _unitParam.fixedFlight;
    ret.fixedWeight = fixedWeight + _unitParam.fixedWeight;
    ret.fixedAttack = fixedAttack + _unitParam.fixedAttack;
    ret.fixedDefence = fixedDefence + _unitParam.fixedDefence;
    ret.fixedMaxHealth = fixedMaxHealth + _unitParam.fixedMaxHealth;

    //成長度
    ret.affectMaxSpeed = affectMaxSpeed * _unitParam.affectMaxSpeed;
    ret.affectAcceleration = affectAcceleration * _unitParam.affectAcceleration;
    ret.affectTurning = affectTurning * _unitParam.affectTurning;
    ret.affectCharge = affectCharge * _unitParam.affectCharge;
    ret.affectFlight = affectFlight * _unitParam.affectFlight;
    ret.affectWeight = affectWeight * _unitParam.affectWeight;
    ret.affectAttack = affectAttack * _unitParam.affectAttack;
    ret.affectDefence = affectDefence * _unitParam.affectDefence;
    ret.affectMaxHealth = affectMaxHealth * _unitParam.affectMaxHealth;

    //止まりやすさ
    ret.chargeBraking = chargeBraking + _unitParam.chargeBraking;

    //制限
    if (ret.chargeBraking > 1.0f)
    {
        ret.chargeBraking = 1.0f;
    }
    else if (ret.chargeBraking < 0.0f)
    {
        ret.chargeBraking = 0.0f;
    }

    //基本チャージ容量
    ret.chargeCapacity = chargeCapacity * _unitParam.chargeCapacity;

    //ブースト関係
    ret.chargeDamp = chargeDamp + _unitParam.chargeDamp;
    ret.boostRate = boostRate + _unitParam.boostRate;
    ret.boostPower = boostPower + _unitParam.boostPower;

    //移動速度
    ret.groundSpeed = groundSpeed + _unitParam.groundSpeed;
    ret.flightSpeed = flightSpeed + _unitParam.flightSpeed;

    return ret;
}