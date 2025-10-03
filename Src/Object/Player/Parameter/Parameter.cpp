#include "Parameter.h"

Parameter::Parameter(void)
{
    maxSpeed_ = 0;
    acceleration_ = 0;
    turning_ = 0;
    charge_ = 0;
    flight_ = 0;
    weight_ = 0;
    attack_ = 0;
    defense_ = 0;
    maxHealth_ = 0;

    affectMaxSpeed_ = 0.0f;
    affectAcceleration_ = 0.0f;
    affectTurning_ = 0.0f;
    affectCharge_= 0.0f;
    affectFlight_ = 0.0f;
    affectWeight_ = 0.0f;
    affectAttack_ = 0.0f;
    affectDefence_ = 0.0f;
    affectMaxHealth_ = 0.0f;

    chargeBraking_ = 0.0f;
    chargeCapacity_ = 0.0f;
}

Parameter::~Parameter(void)
{
}

Parameter Parameter::operator+(const Parameter _param)const
{
    //‰ÁŽZ
    Parameter ret;
    ret.maxSpeed_ = maxSpeed_ + _param.maxSpeed_;
    ret.acceleration_ = acceleration_ + _param.acceleration_;
    ret.turning_ = turning_ + _param.turning_;
    ret.charge_ = charge_ + _param.charge_;
    ret.flight_ = flight_ + _param.flight_;
    ret.weight_ = weight_ + _param.weight_;
    ret.attack_ = attack_ + _param.attack_;
    ret.defense_ = defense_ + _param.defense_;
    ret.maxHealth_ = maxHealth_ + _param.maxHealth_;

    //”{—¦‚È‚Ì‚ÅæŽZ
    ret.affectMaxSpeed_ = affectMaxSpeed_ * _param.affectMaxSpeed_;
    ret.affectAcceleration_ = affectAcceleration_ * _param.affectAcceleration_;
    ret.affectTurning_ = affectTurning_ * _param.affectTurning_;
    ret.affectCharge_ = affectCharge_ * _param.affectCharge_;
    ret.affectFlight_ = affectFlight_ * _param.affectFlight_;
    ret.affectWeight_ = affectWeight_ * _param.affectWeight_;
    ret.affectAttack_ = affectAttack_ * _param.affectAttack_;
    ret.affectDefence_ = affectDefence_ * _param.affectDefence_;
    ret.affectMaxHealth_ = affectMaxHealth_ * _param.affectMaxHealth_;
    
    ret.chargeBraking_ = chargeBraking_ + _param.chargeBraking_;

    //§ŒÀ
    if (ret.chargeBraking_ > 1.0f)
    {
        ret.chargeBraking_ = 1.0f;
    }
    else if(ret.chargeBraking_ < 0.0f)
    {
        ret.chargeBraking_ = 0.0f;
    }

    ret.chargeCapacity_ = chargeCapacity_ + _param.chargeCapacity_;

    return ret;
}

void Parameter::operator+=(const Parameter _param)
{
    //‰ÁŽZ
    maxSpeed_ += _param.maxSpeed_;
    acceleration_ += _param.acceleration_;
    turning_ += _param.turning_;
    charge_ += _param.charge_;
    flight_ += _param.flight_;
    weight_ += _param.weight_;
    attack_ += _param.attack_;
    defense_ += _param.defense_;
    maxHealth_ += _param.maxHealth_;

    //”{—¦‚È‚Ì‚ÅæŽZ
    affectMaxSpeed_ *= _param.affectMaxSpeed_;
    affectAcceleration_ *= _param.affectAcceleration_;
    affectTurning_ *= _param.affectTurning_;
    affectCharge_ *= _param.affectCharge_;
    affectFlight_ *= _param.affectFlight_;
    affectWeight_ *= _param.affectWeight_;
    affectAttack_ *= _param.affectAttack_;
    affectDefence_ *= _param.affectDefence_;
    affectMaxHealth_ *= _param.affectMaxHealth_;

    chargeBraking_ *= _param.chargeBraking_;

    //§ŒÀ
    if (chargeBraking_ > 1.0f)
    {
        chargeBraking_ = 1.0f;
    }
    else if (chargeBraking_ < 0.0f)
    {
        chargeBraking_ = 0.0f;
    }

    chargeCapacity_ += _param.chargeCapacity_;
}
