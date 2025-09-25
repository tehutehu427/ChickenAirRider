#include "Parameter.h"

Parameter::Parameter(void)
{
    maxHealth_ = 0;
    maxSpeed_ = 0;
    acceleration_ = 0;
    attack_ = 0;
    defense_ = 0;
    charge_ = 0;
}

Parameter::~Parameter(void)
{
}

Parameter Parameter::operator+(const Parameter _param)
{
    //‰ÁŽZ
    Parameter ret;
    ret.maxHealth_ = maxHealth_ + _param.maxHealth_;
    ret.maxSpeed_ = maxSpeed_ + _param.maxSpeed_;
    ret.acceleration_ = acceleration_ + _param.acceleration_;
    ret.attack_ = attack_ + _param.attack_;
    ret.defense_ = defense_ + _param.defense_;
    ret.charge_= charge_ + _param.charge_;

    return ret;
}

void Parameter::operator+=(const Parameter _param)
{
    //‰ÁŽZ
    maxHealth_ += _param.maxHealth_;
    maxSpeed_ += _param.maxSpeed_;
    acceleration_ += _param.acceleration_;
    attack_ += _param.attack_;
    defense_ += _param.defense_;
    charge_ += _param.charge_;
}
