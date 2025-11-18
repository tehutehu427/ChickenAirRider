#include "../pch.h"
#include "UnitParameter.h"
#include "Parameter.h"

Parameter Parameter::operator+(const Parameter& _param)const
{
    //加算
    Parameter ret;
    ret.maxSpeed_ = maxSpeed_ + _param.maxSpeed_;
    ret.acceleration_ = acceleration_ + _param.acceleration_;
    ret.turning_ = turning_ + _param.turning_;
    ret.charge_ = charge_ + _param.charge_;
    ret.flight_ = flight_ + _param.flight_;
    ret.weight_ = weight_ + _param.weight_;
    ret.attack_ = attack_ + _param.attack_;
    ret.defence_ = defence_ + _param.defence_;
    ret.maxHealth_ = maxHealth_ + _param.maxHealth_;

    return ret;
}

void Parameter::operator+=(const Parameter& _param)
{
    //加算
    maxSpeed_ += _param.maxSpeed_;
    acceleration_ += _param.acceleration_;
    turning_ += _param.turning_;
    charge_ += _param.charge_;
    flight_ += _param.flight_;
    weight_ += _param.weight_;
    attack_ += _param.attack_;
    defence_ += _param.defence_;
    maxHealth_ += _param.maxHealth_;
}

Parameter Parameter::CalcUnitParam(const UnitParameter& _unitParam)const
{
    //上昇パラメーター * ユニットの成長度 + ユニットの固定パラメーター
    Parameter ret;
    ret.maxSpeed_ = (maxSpeed_ * _unitParam.affectMaxSpeed_) + _unitParam.fixedMaxSpeed_;
    ret.acceleration_ = (acceleration_ * _unitParam.affectAcceleration_) + _unitParam.fixedAcceleration_;
    ret.turning_ = (turning_ * _unitParam.affectTurning_) + _unitParam.fixedTurning_;
    ret.charge_ = (charge_ * _unitParam.affectCharge_) + _unitParam.fixedCharge_;
    ret.flight_ = (flight_ * _unitParam.affectFlight_) + _unitParam.fixedFlight_;
    ret.weight_ = (weight_ * _unitParam.affectWeight_) + _unitParam.fixedWeight_;
    ret.attack_ = (attack_ * _unitParam.affectAttack_) + _unitParam.fixedAttack_;
    ret.defence_ = (defence_ * _unitParam.affectDefence_) + _unitParam.fixedDefence_;
    ret.maxHealth_ = (maxHealth_ * _unitParam.affectMaxHealth_) + _unitParam.fixedMaxHealth_;

    return ret;
}