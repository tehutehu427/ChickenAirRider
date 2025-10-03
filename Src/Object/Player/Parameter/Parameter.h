#pragma once

class Parameter
{
public:

	//コンストラクタ
	Parameter(void);

	//デストラクタ
	~Parameter(void);

	//上昇するパラメーター関係
	int maxSpeed_;		//最高速度
	int acceleration_;	//加速度
	int turning_;		//旋回力
	int charge_;		//チャージ速度
	int flight_;		//飛行力
	int weight_;		//重量
	int attack_;		//攻撃力
	int defense_;		//防御力
	int maxHealth_;		//最大体力

	//各キャラ、機体ごとの影響度(伸びやすさ)
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
	Parameter operator+(const Parameter _param)const;
	void operator+=(const Parameter _param);
};

