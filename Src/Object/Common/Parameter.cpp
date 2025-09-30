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
}
