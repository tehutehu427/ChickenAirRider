#include "UnitParameter.h"

UnitParameter UnitParameter::Calculate(const UnitParameter& _unitParam) const
{
    UnitParameter ret;

    //固定値
    ret.fixedMaxSpeed_ = fixedMaxSpeed_ + _unitParam.fixedMaxSpeed_;
    ret.fixedAcceleration_ = fixedAcceleration_ + _unitParam.fixedAcceleration_;
    ret.fixedTurning_ = fixedTurning_ + _unitParam.fixedTurning_;
    ret.fixedCharge_ = fixedCharge_ + _unitParam.fixedCharge_;
    ret.fixedFlight_ = fixedFlight_ + _unitParam.fixedFlight_;
    ret.fixedWeight_ = fixedWeight_ + _unitParam.fixedWeight_;
    ret.fixedAttack_ = fixedAttack_ + _unitParam.fixedAttack_;
    ret.fixedDefence_ = fixedDefence_ + _unitParam.fixedDefence_;
    ret.fixedMaxHealth_ = fixedMaxHealth_ + _unitParam.fixedMaxHealth_;

    //成長度
    ret.affectMaxSpeed_ = affectMaxSpeed_ * _unitParam.affectMaxSpeed_;
    ret.affectAcceleration_ = affectAcceleration_ * _unitParam.affectAcceleration_;
    ret.affectTurning_ = affectTurning_ * _unitParam.affectTurning_;
    ret.affectCharge_ = affectCharge_ * _unitParam.affectCharge_;
    ret.affectFlight_ = affectFlight_ * _unitParam.affectFlight_;
    ret.affectWeight_ = affectWeight_ * _unitParam.affectWeight_;
    ret.affectAttack_ = affectAttack_ * _unitParam.affectAttack_;
    ret.affectDefence_ = affectDefence_ * _unitParam.affectDefence_;
    ret.affectMaxHealth_ = affectMaxHealth_ * _unitParam.affectMaxHealth_;

    //止まりやすさ
    ret.chargeBraking_ = chargeBraking_ + _unitParam.chargeBraking_;

    //制限
    if (ret.chargeBraking_ > 1.0f)
    {
        ret.chargeBraking_ = 1.0f;
    }
    else if (ret.chargeBraking_ < 0.0f)
    {
        ret.chargeBraking_ = 0.0f;
    }

    //基本チャージ容量
    ret.chargeCapacity_ = chargeCapacity_ * _unitParam.chargeCapacity_;

    //ブースト関係
    ret.chargeDamp_ = chargeDamp_ + _unitParam.chargeDamp_;
    ret.boostRate_ = boostRate_ + _unitParam.boostRate_;
    ret.boostPower_ = boostPower_ + _unitParam.boostPower_;

    return ret;
}