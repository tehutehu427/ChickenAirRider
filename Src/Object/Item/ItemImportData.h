#pragma once
#include<DxLib.h>
#include<string>
#include<vector>
#include<iostream>
#include"../Lib/nlohmann/json.hpp"
#include"../Player/Parameter/Parameter.h"

struct ItemImportData
{
	//名前
	std::string name;

	//パラメーター
	Parameter param;

	//当たり判定情報(必要なもののみ格納)
	std::string geometry;			//形状名
	std::string tag;				//タグ
	float radius;					//半径
};

//Json呼び出し
inline void FromJson(const nlohmann::json& _j, ItemImportData& _data)
{
	//名前
	if (_j.contains("name"))
	{
		_data.name = _j.value("name", "");
	}
	else
	{
		std::cerr << "ItemのJsonファイルにnameが存在しません" << "\n";
		return;
	}

	//パラメーター
	if (_j.contains("Parameter"))
	{
		const auto& parameter = _j.at("Parameter");

		//各パラメータ
		if (parameter.contains("maxSpeed"))
		{
			_data.param.maxSpeed_ = parameter.value("maxSpeed", 0);
		}
		if (parameter.contains("acceleration"))
		{
			_data.param.acceleration_ = parameter.value("acceleration", 0);
		}
		if (parameter.contains("turning"))
		{
			_data.param.turning_ = parameter.value("turning", 0);
		}
		if (parameter.contains("charge"))
		{
			_data.param.charge_ = parameter.value("charge", 0);
		}
		if (parameter.contains("flight"))
		{
			_data.param.flight_ = parameter.value("flight", 0);
		}
		if (parameter.contains("weight"))
		{
			_data.param.weight_ = parameter.value("weight", 0);
		}
		if (parameter.contains("attack"))
		{
			_data.param.attack_ = parameter.value("attack", 0);
		}
		if (parameter.contains("defence"))
		{
			_data.param.defense_ = parameter.value("defense", 0);
		}
		if (parameter.contains("maxHealth"))
		{
			_data.param.maxHealth_ = parameter.value("maxHealth", 0);
		}
	}

	//形状名
	if (_j.contains("geometry"))
	{
		_data.geometry = _j.value("geometry", "");

		if (_j.contains("tag"))
		{
			_data.tag = _j.value("tag", "");
		}
		else
		{
			std::cerr << "ItemのJsonファイルにtagが存在しません" << "\n";
			return;
		}

		//半径
		if (_j.contains("radius"))
		{
			_data.radius = _j.value("radius", 1.0f);
		}
	}
}

inline void FromCsv(std::stringstream& _ss, ItemImportData& _s)
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
