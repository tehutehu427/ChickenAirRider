#pragma once
#include"../pch.h"

struct UnitParameter
{
	//ユニット名
	std::string name_;

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
	float chargeDamp_ = 0.0f;		//チャージの影響度(1.0だとチャージそのまま)
	float boostRate_ = 0.0f;		//チャージ満タン時の最高速度超過率(%換算,0.0だと最高速度そのまま)
	int boostPower_ = 0;			//ブーストカーブの急さ(値が大きいとチャージしきった時の影響が大きい)

	//演算
	UnitParameter Calculate(const UnitParameter& _unitParam)const;
};

//Json呼び出し
inline void FromJson(const nlohmann::json& _j, UnitParameter& _p)
{
	_p.name_ = _j["name"];
	_p.fixedMaxSpeed_ = _j["fixedMaxSpeed"];
	_p.fixedAcceleration_ = _j["fixedAcceleration"];
	_p.fixedTurning_ = _j["fixedTurning"];
	_p.fixedCharge_ = _j["fixedCharge"];
	_p.fixedFlight_ = _j["fixedFlight"];
	_p.fixedWeight_ = _j["fixedWeight"];
	_p.fixedAttack_ = _j["fixedAttack"];
	_p.fixedDefence_ = _j["fixedDefence"];
	_p.fixedMaxHealth_ = _j["fixedMaxHealth"];

	_p.affectMaxSpeed_ = _j["affectMaxSpeed"];
	_p.affectAcceleration_ = _j["affectAcceleration"];
	_p.affectTurning_ = _j["affectTurning"];
	_p.affectCharge_ = _j["affectCharge"];
	_p.affectFlight_ = _j["affectFlight"];
	_p.affectWeight_ = _j["affectWeight"];
	_p.affectAttack_ = _j["affectAttack"];
	_p.affectDefence_ = _j["affectDefence"];
	_p.affectMaxHealth_ = _j["affectMaxHealth"];

	_p.chargeBraking_ = _j["chargeBraking"];
	_p.chargeCapacity_ = _j["chargeCapacity"];
	_p.chargeDamp_ = _j["chargeDamp"];
	_p.boostRate_ = _j["boostRate"];
	_p.boostPower_ = _j["boostPower"];
}

inline void FromCsv(std::stringstream& _ss, UnitParameter& _p) 
{
	//名前
	std::getline(_ss, _p.name_, ',');
	std::string fixedMaxSpeed, fixedAcceleration, fixedTurning, fixedCharge, fixedFlight, fixedWeight, fixedAttack, fixedDefence, fixedMaxHealth,
		affectMaxSpeed, affectAcceleration, affectTurning, affectCharge, affectFlight, affectWeight, affectAttack, affectDefence, affectMaxHealth,
		chargeBraking, chargeCapacity, chargeDamp, boostRate, boostPower;
	
	//変数に格納
	std::getline(_ss, fixedMaxSpeed, ',');
	std::getline(_ss, fixedAcceleration, ',');
	std::getline(_ss, fixedTurning, ',');
	std::getline(_ss, fixedCharge, ',');
	std::getline(_ss, fixedFlight, ',');
	std::getline(_ss, fixedWeight, ',');
	std::getline(_ss, fixedAttack, ',');
	std::getline(_ss, fixedDefence, ',');
	std::getline(_ss, fixedMaxHealth, ',');
	std::getline(_ss, affectMaxSpeed, ',');
	std::getline(_ss, affectAcceleration, ',');
	std::getline(_ss, affectTurning, ',');
	std::getline(_ss, affectCharge, ',');
	std::getline(_ss, affectFlight, ',');
	std::getline(_ss, affectWeight, ',');
	std::getline(_ss, affectAttack, ',');
	std::getline(_ss, affectDefence, ',');
	std::getline(_ss, affectMaxHealth, ',');
	std::getline(_ss, chargeBraking, ',');
	std::getline(_ss, chargeCapacity, ',');
	std::getline(_ss, chargeDamp, ',');
	std::getline(_ss, boostRate, ',');
	std::getline(_ss, boostPower, ',');
	
	//適用
	_p.fixedMaxSpeed_ = std::stoi(fixedMaxSpeed);
	_p.fixedAcceleration_ = std::stoi(fixedAcceleration);
	_p.fixedTurning_ = std::stof(fixedTurning);
	_p.fixedCharge_ = std::stof(fixedCharge);
	_p.fixedFlight_ = std::stof(fixedFlight);
	_p.fixedWeight_ = std::stof(fixedWeight);
	_p.fixedAttack_ = std::stof(fixedAttack);
	_p.fixedDefence_ = std::stof(fixedDefence);
	_p.fixedMaxHealth_ = std::stof(fixedMaxHealth);
	_p.affectMaxSpeed_ = std::stof(affectMaxSpeed);
	_p.affectAcceleration_ = std::stof(affectAcceleration);
	_p.affectTurning_ = std::stof(affectTurning);
	_p.affectCharge_ = std::stof(affectCharge);
	_p.affectFlight_ = std::stof(affectFlight);
	_p.affectWeight_ = std::stof(affectWeight);
	_p.affectAttack_ = std::stof(affectAttack);
	_p.affectDefence_ = std::stof(affectDefence);
	_p.affectMaxHealth_ = std::stof(affectMaxHealth);
	_p.chargeBraking_ = std::stof(chargeBraking);
	_p.chargeCapacity_ = std::stof(chargeCapacity);
	_p.chargeDamp_ = std::stof(chargeDamp);
	_p.boostRate_ = std::stof(boostRate);
	_p.boostPower_ = std::stof(boostPower);
}