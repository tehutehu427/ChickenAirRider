#pragma once

struct UnitParameter
{
	//各ユニットの固定パラメータ―
	float fixedMaxSpeed_ = 0.0f;		//固定最高速
	float fixedAcceleration_ = 0.0f;	//固定加速度
	float fixedTurning_ = 0.0f;			//固定旋回力
	float fixedCharge_ = 0.0f;			//固定チャージ速度
	float fixedFlight_ = 0.0f;			//固定飛行力
	float fixedWeight_ = 0.0f;			//固定重量
	float fixedAttack_ = 0.0f;			//固定攻撃力
	float fixedDefence_ = 0.0f;			//固定防御力
	float fixedMaxHealth_ = 0.0f;		//固定最大体力

	//各ユニットごとの影響度(伸びやすさ)
	float affectMaxSpeed_ = 0.0f;		//最高速度影響度
	float affectAcceleration_ = 0.0f;	//加速度影響度
	float affectTurning_ = 0.0f;		//旋回力影響度
	float affectCharge_ = 0.0f;			//チャージ速度影響度
	float affectFlight_ = 0.0f;			//飛行力影響度
	float affectWeight_ = 0.0f;			//重量影響度
	float affectAttack_ = 0.0f;			//攻撃力影響度
	float affectDefence_ = 0.0f;		//防御力影響度
	float affectMaxHealth_ = 0.0f;		//最大体力影響度

	//チャージ
	float chargeBraking_ = 0.0f;	//チャージ時の止まりやすさ(0.0f～1.0f)
	float chargeCapacity_ = 0.0f;	//チャージ容量

	//演算
	UnitParameter Calculate(const UnitParameter& _unitParam)const;
};