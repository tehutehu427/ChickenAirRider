#pragma once
#include"UnitParameter.h"

struct Parameter
{
	//上昇するパラメーター関係
	float maxSpeed_ = 0.0f;		//最高速度
	float acceleration_ = 0.0f;	//加速度
	float turning_ = 0.0f;		//旋回力
	float charge_ = 0.0f;		//チャージ速度
	float flight_ = 0.0f;		//飛行力
	float weight_ = 0.0f;		//重量
	float attack_ = 0.0f;		//攻撃力
	float defence_ = 0.0f;		//防御力
	float maxHealth_ = 0.0f;	//最大体力

	//演算
	Parameter operator+(const Parameter& _param)const;
	void operator+=(const Parameter& _param);

	//ユニット達との演算
	Parameter CalcUnitParam(const UnitParameter& _unitParam)const;
};

