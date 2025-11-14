#pragma once
#include<DxLib.h>
#include<string>
#include<vector>
#include<iostream>
#include"../Lib/nlohmann/json.hpp"
#include"../Player/Parameter/Parameter.h"

struct BoxCreatePositionData
{
	VECTOR pos;
};

//Json呼び出し
inline void FromJson(const nlohmann::json& _j, BoxCreatePositionData& _data)
{
	//名前
	if (_j.contains("position"))
	{
		const auto& pos = _j.at("position");

		_data.pos.x = pos.value("x", 0.0f);
		_data.pos.y = pos.value("y", 0.0f);
		_data.pos.z = pos.value("z", 0.0f);
	}
	else
	{
		std::cerr << "Jsonファイルにposが存在しません" << "\n";
		return;
	}
}

inline void FromCsv(std::stringstream& _ss, BoxCreatePositionData& _s)
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