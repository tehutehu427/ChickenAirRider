#pragma once

class UnitParameter
{
public:

	//コンストラクタ
	UnitParameter(void);

	//デストラクタ
	~UnitParameter(void);

	//各ユニットごとの影響度(伸びやすさ)
	float affectMaxSpeed_;		//最高速度影響度
	float affectAcceleration_;	//加速度影響度
	float affectTurning_;		//旋回力影響度
	float affectCharge_;		//チャージ速度影響度
	float affectFlight_;		//飛行力影響度
	float affectWeight_;		//重量影響度
	float affectAttack_;		//攻撃力影響度
	float affectDefence_;		//防御力影響度
	float affectMaxHealth_;		//最大体力影響度

	//チャージ
	float chargeBraking_;	//チャージ時の止まりやすさ(0.0f～1.0f)
	float chargeCapacity_;	//チャージ容量

	//演算
	UnitParameter& operator*(const UnitParameter& _unitParam)const;
	void operator*=(const UnitParameter& _unitParam);
};

