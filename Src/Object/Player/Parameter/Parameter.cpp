#include "../pch.h"
#include "UnitParameter.h"
#include "Parameter.h"

Parameter Parameter::operator+(const Parameter& _param)const
{
    //加算
    Parameter ret;
    ret.maxSpeed = maxSpeed + _param.maxSpeed;
    ret.acceleration = acceleration + _param.acceleration;
    ret.turning = turning + _param.turning;
    ret.charge = charge + _param.charge;
    ret.flight = flight + _param.flight;
    ret.weight = weight + _param.weight;
    ret.attack = attack + _param.attack;
    ret.defence = defence + _param.defence;
    ret.maxHealth = maxHealth + _param.maxHealth;

    return ret;
}

void Parameter::operator+=(const Parameter& _param)
{
    //加算
    maxSpeed += _param.maxSpeed;
    acceleration += _param.acceleration;
    turning += _param.turning;
    charge += _param.charge;
    flight += _param.flight;
    weight += _param.weight;
    attack += _param.attack;
    defence += _param.defence;
    maxHealth += _param.maxHealth;
}

Parameter Parameter::CalcUnitParam(const UnitParameter& _unitParam)const
{
    //上昇パラメーター * ユニットの成長度 + ユニットの固定パラメーター
    Parameter ret;
    ret.maxSpeed = (maxSpeed * _unitParam.affectMaxSpeed) + _unitParam.fixedMaxSpeed;
    ret.acceleration = (acceleration * _unitParam.affectAcceleration) + _unitParam.fixedAcceleration;
    ret.turning = (turning * _unitParam.affectTurning) + _unitParam.fixedTurning;
    ret.charge = (charge * _unitParam.affectCharge) + _unitParam.fixedCharge;
    ret.flight = (flight * _unitParam.affectFlight) + _unitParam.fixedFlight;
    ret.weight = (weight * _unitParam.affectWeight) + _unitParam.fixedWeight;
    ret.attack = (attack * _unitParam.affectAttack) + _unitParam.fixedAttack;
    ret.defence = (defence * _unitParam.affectDefence) + _unitParam.fixedDefence;
    ret.maxHealth = (maxHealth * _unitParam.affectMaxHealth) + _unitParam.fixedMaxHealth;

    return ret;
}

const float Parameter::GetHealthValue(void) const
{
    return maxHealth * MAX_HEALTH_VALUE;
}

void Parameter::Clamp(void)
{
    //最大値
    const float maxParam = static_cast<float>(Parameter::MAX_PARAM);

    //最小値
    const float minParam = static_cast<float>(Parameter::MIN_PARAM);

    //上下限値
    maxSpeed = std::clamp(maxSpeed, minParam, maxParam);
    acceleration = std::clamp(acceleration, minParam, maxParam);
    turning = std::clamp(turning, minParam, maxParam);
    charge = std::clamp(charge, minParam, maxParam);
    flight = std::clamp(flight, minParam, maxParam);
    weight = std::clamp(weight, minParam, maxParam);
    attack = std::clamp(attack, minParam, maxParam);
    defence = std::clamp(defence, minParam, maxParam);
    maxHealth = std::clamp(maxHealth, minParam, maxParam);
}
