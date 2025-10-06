#include "UnitParameter.h"

UnitParameter::UnitParameter(void)
{
    affectMaxSpeed_ = 0.0f;
    affectAcceleration_ = 0.0f;
    affectTurning_ = 0.0f;
    affectCharge_ = 0.0f;
    affectFlight_ = 0.0f;
    affectWeight_ = 0.0f;
    affectAttack_ = 0.0f;
    affectDefence_ = 0.0f;
    affectMaxHealth_ = 0.0f;

    chargeBraking_ = 0.0f;
    chargeCapacity_ = 0.0f;
}

UnitParameter::~UnitParameter(void)
{
}

UnitParameter& UnitParameter::operator*(const UnitParameter& _unitParam) const
{
    UnitParameter ret;

    //”{—¦‚È‚Ì‚ÅæŽZ
    ret.affectMaxSpeed_ = affectMaxSpeed_ * _unitParam.affectMaxSpeed_;
    ret.affectAcceleration_ = affectAcceleration_ * _unitParam.affectAcceleration_;
    ret.affectTurning_ = affectTurning_ * _unitParam.affectTurning_;
    ret.affectCharge_ = affectCharge_ * _unitParam.affectCharge_;
    ret.affectFlight_ = affectFlight_ * _unitParam.affectFlight_;
    ret.affectWeight_ = affectWeight_ * _unitParam.affectWeight_;
    ret.affectAttack_ = affectAttack_ * _unitParam.affectAttack_;
    ret.affectDefence_ = affectDefence_ * _unitParam.affectDefence_;
    ret.affectMaxHealth_ = affectMaxHealth_ * _unitParam.affectMaxHealth_;

    ret.chargeBraking_ = chargeBraking_ + _unitParam.chargeBraking_;

    //§ŒÀ
    if (ret.chargeBraking_ > 1.0f)
    {
        ret.chargeBraking_ = 1.0f;
    }
    else if (ret.chargeBraking_ < 0.0f)
    {
        ret.chargeBraking_ = 0.0f;
    }

    ret.chargeCapacity_ = chargeCapacity_ * _unitParam.chargeCapacity_;

    return ret;
}

void UnitParameter::operator*=(const UnitParameter& _unitParam)
{
    //”{—¦‚È‚Ì‚ÅæŽZ
    affectMaxSpeed_ *= _unitParam.affectMaxSpeed_;
    affectAcceleration_ *= _unitParam.affectAcceleration_;
    affectTurning_ *= _unitParam.affectTurning_;
    affectCharge_ *= _unitParam.affectCharge_;
    affectFlight_ *= _unitParam.affectFlight_;
    affectWeight_ *= _unitParam.affectWeight_;
    affectAttack_ *= _unitParam.affectAttack_;
    affectDefence_ *= _unitParam.affectDefence_;
    affectMaxHealth_ *= _unitParam.affectMaxHealth_;

    chargeBraking_ *= _unitParam.chargeBraking_;

    //§ŒÀ
    if (chargeBraking_ > 1.0f)
    {
        chargeBraking_ = 1.0f;
    }
    else if (chargeBraking_ < 0.0f)
    {
        chargeBraking_ = 0.0f;
    }

    chargeCapacity_ += _unitParam.chargeCapacity_;
}
