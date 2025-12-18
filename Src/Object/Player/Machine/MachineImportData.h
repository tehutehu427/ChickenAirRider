#pragma once
#include<DxLib.h>
#include"../Lib/nlohmann/json.hpp"
#include<iostream>
#include<unordered_map>
#include<string>
#include"../Parameter/UnitParameter.h"

struct MachineImportData
{
	//機体名
	std::string name;

	//アニメーションの有無
	bool isAnim;

	//当たり判定半径
	float hitRadius;

	//モデルの大きさ
	VECTOR scale;

	//固定パラメーター
	UnitParameter param;
};

//Json呼び出し
inline void FromJson(const nlohmann::json& _j, MachineImportData& _data)
{
	//名前
	if (_j.contains("name"))
	{
		_data.name = _j.value("name", "");
	}
	else
	{
		std::cerr << "CharacterのJsonファイルにnameが存在しません" << "\n";
		return;
	}

	//アニメーションの有無
	if (_j.contains("isAnim"))
	{
		_data.isAnim = _j.value("isAnim", false);
	}

	//半径
	if (_j.contains("radius"))
	{
		_data.hitRadius = _j.value("radius", 0.0f);
	}

	//モデルの大きさ
	if (_j.contains("scale"))
	{
		_data.scale.x = _j["scale"].value("x", 1.0f);
		_data.scale.y = _j["scale"].value("y", 1.0f);
		_data.scale.z = _j["scale"].value("z", 1.0f);
	}

	//パラメーター
	if (_j.contains("parameter"))
	{
		//固定値
		_data.param.fixedMaxSpeed = _j["parameter"].value("fixedMaxSpeed",0.0f);
		_data.param.fixedAcceleration = _j["parameter"].value("fixedAcceleration",0.0f);
		_data.param.fixedCharge = _j["parameter"].value("fixedCharge",0.0f);
		_data.param.fixedTurning = _j["parameter"].value("fixedTurning",0.0f);
		_data.param.fixedFlight = _j["parameter"].value("fixedFlight",0.0f);
		_data.param.fixedWeight = _j["parameter"].value("fixedWeight",0.0f);
		_data.param.fixedAttack = _j["parameter"].value("fixedAttack",0.0f);
		_data.param.fixedDefence = _j["parameter"].value("fixedDefence",0.0f);
		_data.param.fixedMaxHealth = _j["parameter"].value("fixedMaxHealth",0.0f);

		//影響度
		_data.param.affectMaxSpeed = _j["parameter"].value("affectMaxSpeed",0.0f);
		_data.param.affectAcceleration = _j["parameter"].value("affectAcceleration",0.0f);
		_data.param.affectCharge = _j["parameter"].value("affectCharge",0.0f);
		_data.param.affectTurning = _j["parameter"].value("affectTurning",0.0f);
		_data.param.affectFlight = _j["parameter"].value("affectFlight",0.0f);
		_data.param.affectWeight = _j["parameter"].value("affectWeight",0.0f);
		_data.param.affectAttack = _j["parameter"].value("affectAttack",0.0f);
		_data.param.affectDefence = _j["parameter"].value("affectDefence",0.0f);
		_data.param.affectMaxHealth = _j["parameter"].value("affectMaxHealth",0.0f);

		//チャージ
		_data.param.chargeBraking = _j["parameter"].value("chargeBraking",0.0f);
		_data.param.chargeCapacity = _j["parameter"].value("chargeCapacity",0.0f);
		_data.param.chargeDamp = _j["parameter"].value("chargeDamp",0.0f);
		_data.param.boostRate = _j["parameter"].value("boostRate",0.0f);
		_data.param.boostPower = _j["parameter"].value("boostPower",0.0f);

		//速度
		_data.param.groundSpeed = _j["parameter"].value("groundSpeed",0.0f);
		_data.param.flightSpeed = _j["parameter"].value("flightSpeed",0.0f);
	}
}

inline void FromCsv(std::stringstream& _ss, MachineImportData& _s)
{
	////格納内容
	//std::string token;

	////インポート

	////名前
	//std::getline(_ss, _data.name, ',');

	////座標
	//std::getline(_ss, token, ',');	_data.position.x = std::stof(token);
	//std::getline(_ss, token, ',');	_data.position.y = std::stof(token);
	//std::getline(_ss, token, ',');	_data.position.z = std::stof(token);

	////大きさ
	//std::getline(_ss, token, ',');	_data.scale.x = std::stof(token);
	//std::getline(_ss, token, ',');	_data.scale.y = std::stof(token);
	//std::getline(_ss, token, ',');	_data.scale.z = std::stof(token);

	////回転
	//std::getline(_ss, token, ',');	_data.quaternion.w = std::stof(token);
	//std::getline(_ss, token, ',');	_data.quaternion.x = std::stof(token);
	//std::getline(_ss, token, ',');	_data.quaternion.y = std::stof(token);
	//std::getline(_ss, token, ',');	_data.quaternion.z = std::stof(token);
}