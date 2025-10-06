#pragma once
#include"UnitParameter.h"

class Parameter
{
public:

	//コンストラクタ
	Parameter(void);

	//デストラクタ
	~Parameter(void);

	//上昇するパラメーター関係
	float maxSpeed_;		//最高速度
	float acceleration_;	//加速度
	float turning_;			//旋回力
	float charge_;			//チャージ速度
	float flight_;			//飛行力
	float weight_;			//重量
	float attack_;			//攻撃力
	float defense_;			//防御力
	float maxHealth_;		//最大体力

	//演算
	Parameter& operator+(const Parameter& _param)const;
	void operator+=(const Parameter& _param);

	//ユニット達との演算
	Parameter& operator*(const UnitParameter& _unitParam)const;
};

