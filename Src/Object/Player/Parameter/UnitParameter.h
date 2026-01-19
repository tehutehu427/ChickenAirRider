#pragma once
#include <string>
#include"../Lib/nlohmann/json.hpp"

struct UnitParameter
{
	//ユニット名
	std::string name;

	//各ユニットの固定パラメータ―
	float fixedMaxSpeed = 0.0f;			//固定最高速
	float fixedAcceleration = 0.0f;		//固定加速度
	float fixedTurning = 0.0f;			//固定旋回力
	float fixedCharge = 0.0f;			//固定チャージ速度
	float fixedFlight = 0.0f;			//固定飛行力
	float fixedWeight = 0.0f;			//固定重量
	float fixedAttack = 0.0f;			//固定攻撃力
	float fixedDefence = 0.0f;			//固定防御力
	float fixedMaxHealth = 0.0f;		//固定最大体力

	//各ユニットごとの影響度(伸びやすさ)
	float affectMaxSpeed = 0.0f;		//最高速度影響度
	float affectAcceleration = 0.0f;	//加速度影響度
	float affectTurning = 0.0f;			//旋回力影響度
	float affectCharge = 0.0f;			//チャージ速度影響度
	float affectFlight = 0.0f;			//飛行力影響度
	float affectWeight = 0.0f;			//重量影響度
	float affectAttack = 0.0f;			//攻撃力影響度
	float affectDefence = 0.0f;			//防御力影響度
	float affectMaxHealth = 0.0f;		//最大体力影響度

	//チャージ
	float chargeBraking = 0.0f;		//チャージ時の止まりやすさ(0.0f～1.0f)
	float chargeCapacity = 0.0f;	//チャージ容量
	float chargeDamp = 0.0f;		//チャージの影響度(1.0だとチャージそのまま)
	float boostRate = 0.0f;			//チャージ満タン時の最高速度超過率(%換算,0.0だと最高速度そのまま)
	int boostPower = 0;				//ブーストカーブの急さ(値が大きいとチャージしきった時の影響が大きい)
	float rateOfConvergence = 0.0f;	//速度収束率

	//地上と飛行の速度倍率
	float groundSpeed = 0.0f;
	float flightSpeed = 0.0f;

	//演算
	UnitParameter Calculate(const UnitParameter& _unitParam)const;
};

////Json呼び出し
//inline void FromJson(const nlohmann::json& _j, UnitParameter& _p)
//{
//	_p.name = _j["name"];
//	_p.fixedMaxSpeed = _j["fixedMaxSpeed"];
//	_p.fixedAcceleration = _j["fixedAcceleration"];
//	_p.fixedTurning = _j["fixedTurning"];
//	_p.fixedCharge = _j["fixedCharge"];
//	_p.fixedFlight = _j["fixedFlight"];
//	_p.fixedWeight = _j["fixedWeight"];
//	_p.fixedAttack = _j["fixedAttack"];
//	_p.fixedDefence = _j["fixedDefence"];
//	_p.fixedMaxHealth = _j["fixedMaxHealth"];
//
//	_p.affectMaxSpeed = _j["affectMaxSpeed"];
//	_p.affectAcceleration = _j["affectAcceleration"];
//	_p.affectTurning = _j["affectTurning"];
//	_p.affectCharge = _j["affectCharge"];
//	_p.affectFlight = _j["affectFlight"];
//	_p.affectWeight = _j["affectWeight"];
//	_p.affectAttack = _j["affectAttack"];
//	_p.affectDefence = _j["affectDefence"];
//	_p.affectMaxHealth = _j["affectMaxHealth"];
//
//	_p.chargeBraking = _j["chargeBraking"];
//	_p.chargeCapacity = _j["chargeCapacity"];
//	_p.chargeDamp = _j["chargeDamp"];
//	_p.boostRate = _j["boostRate"];
//	_p.boostPower = _j["boostPower"];
//}
//
//inline void FromCsv(std::stringstream& _ss, UnitParameter& _p) 
//{
//	//名前
//	std::getline(_ss, _p.name, ',');
//	std::string fixedMaxSpeed, fixedAcceleration, fixedTurning, fixedCharge, fixedFlight, fixedWeight, fixedAttack, fixedDefence, fixedMaxHealth,
//		affectMaxSpeed, affectAcceleration, affectTurning, affectCharge, affectFlight, affectWeight, affectAttack, affectDefence, affectMaxHealth,
//		chargeBraking, chargeCapacity, chargeDamp, boostRate, boostPower;
//	
//	//変数に格納
//	std::getline(_ss, fixedMaxSpeed, ',');
//	std::getline(_ss, fixedAcceleration, ',');
//	std::getline(_ss, fixedTurning, ',');
//	std::getline(_ss, fixedCharge, ',');
//	std::getline(_ss, fixedFlight, ',');
//	std::getline(_ss, fixedWeight, ',');
//	std::getline(_ss, fixedAttack, ',');
//	std::getline(_ss, fixedDefence, ',');
//	std::getline(_ss, fixedMaxHealth, ',');
//	std::getline(_ss, affectMaxSpeed, ',');
//	std::getline(_ss, affectAcceleration, ',');
//	std::getline(_ss, affectTurning, ',');
//	std::getline(_ss, affectCharge, ',');
//	std::getline(_ss, affectFlight, ',');
//	std::getline(_ss, affectWeight, ',');
//	std::getline(_ss, affectAttack, ',');
//	std::getline(_ss, affectDefence, ',');
//	std::getline(_ss, affectMaxHealth, ',');
//	std::getline(_ss, chargeBraking, ',');
//	std::getline(_ss, chargeCapacity, ',');
//	std::getline(_ss, chargeDamp, ',');
//	std::getline(_ss, boostRate, ',');
//	std::getline(_ss, boostPower, ',');
//	
//	//適用
//	_p.fixedMaxSpeed = std::stoi(fixedMaxSpeed);
//	_p.fixedAcceleration = std::stoi(fixedAcceleration);
//	_p.fixedTurning = std::stof(fixedTurning);
//	_p.fixedCharge = std::stof(fixedCharge);
//	_p.fixedFlight = std::stof(fixedFlight);
//	_p.fixedWeight = std::stof(fixedWeight);
//	_p.fixedAttack = std::stof(fixedAttack);
//	_p.fixedDefence = std::stof(fixedDefence);
//	_p.fixedMaxHealth = std::stof(fixedMaxHealth);
//	_p.affectMaxSpeed = std::stof(affectMaxSpeed);
//	_p.affectAcceleration = std::stof(affectAcceleration);
//	_p.affectTurning = std::stof(affectTurning);
//	_p.affectCharge = std::stof(affectCharge);
//	_p.affectFlight = std::stof(affectFlight);
//	_p.affectWeight = std::stof(affectWeight);
//	_p.affectAttack = std::stof(affectAttack);
//	_p.affectDefence = std::stof(affectDefence);
//	_p.affectMaxHealth = std::stof(affectMaxHealth);
//	_p.chargeBraking = std::stof(chargeBraking);
//	_p.chargeCapacity = std::stof(chargeCapacity);
//	_p.chargeDamp = std::stof(chargeDamp);
//	_p.boostRate = std::stof(boostRate);
//	_p.boostPower = std::stof(boostPower);
//}