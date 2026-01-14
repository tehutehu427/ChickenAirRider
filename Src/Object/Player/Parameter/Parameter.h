#pragma once
#include"UnitParameter.h"

struct Parameter
{
	//パラメーター関係
	static constexpr float WEIGHT_AFFECT = 0.2f;		//攻防の重さの影響度
	static constexpr float FLIGHT_AFFECT = 0.1f;		//攻防の飛行の影響度
	static constexpr float DEFENCE_AFFECT = 0.5f;		//防御の基本影響値
	static constexpr int MAX_PARAM = 15;				//上昇最大値
	static constexpr float MAX_HEALTH_VALUE = 30.0f;	//基礎最大体力値

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

	//HPの値の取得
	const float GetHealthValue(void)const;
};

