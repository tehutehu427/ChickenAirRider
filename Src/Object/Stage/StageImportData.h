#pragma once
#include<DxLib.h>
#include<string>
#include<vector>
#include"../Lib/nlohmann/json.hpp"
#include"../Common/Quaternion.h"

//外部インポート用
struct StageImportData
{
	//基本情報
	std::string name;		//モデル名
	VECTOR position;		//座標
	VECTOR scale;			//大きさ
	Quaternion quaternion;	//回転

	//当たり判定情報(必要なもののみ格納)
	std::string geometry;			//形状名
	std::string tag;				//タグ
	float broudRadius;				//当たり判定前半径
	float radius;					//半径
	VECTOR localPos1;				//相対座標１
	VECTOR localPos2;				//相対座標２

private:

	//外部管理用
	friend void FromJson(const nlohmann::json&, StageImportData&);
	friend void FromCsv(std::stringstream& _ss, StageImportData&);
};

//Json呼び出し
inline void FromJson(const nlohmann::json& _j, StageImportData& _data)
{
	//名前
	if (_j.contains("name"))
	{
		_data.name = _j.value("name", "");
	}
	else
	{
		std::cerr << "StageObjectのJsonファイルにnameが存在しません" << "\n";
		return;
	}

	//座標
	if (_j.contains("position"))
	{
		const auto& pos = _j.at("position");
		_data.position.x = pos.value("x", 0.0f);
		_data.position.y = pos.value("y", 0.0f);
		_data.position.z = pos.value("z", 0.0f);
	}

	//大きさ
	if (_j.contains("scale"))
	{
		const auto& scl = _j.at("scale");
		_data.scale.x = scl.value("x", 1.0f);
		_data.scale.y = scl.value("y", 1.0f);
		_data.scale.z = scl.value("z", 1.0f);
	}

	//回転
	if (_j.contains("quaternion"))
	{
		const auto& q = _j.at("quaternion");
		_data.quaternion.w = q.value("w", 1.0f);
		_data.quaternion.x = q.value("x", 0.0f);
		_data.quaternion.y = q.value("y", 0.0f);
		_data.quaternion.z = q.value("z", 0.0f);
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
			std::cerr << "StageObjectのJsonファイルにtagが存在しません" << "\n";
			return;
		}

		//当たり判定前
		if (_j.contains("broudRadius"))
		{
			_data.broudRadius = _j.value("broudRadius", 1.0f);
		}

		//半径
		if (_j.contains("radius"))
		{
			_data.radius = _j.value("radius", 1.0f);
		}

		//相対座標1
		if (_j.contains("localPos1"))
		{
			const auto& pos = _j.at("localPos1");
			_data.localPos1.x = pos.value("x", 0.0f);
			_data.localPos1.y = pos.value("y", 0.0f);
			_data.localPos1.z = pos.value("z", 0.0f);
		}
		//相対座標1
		if (_j.contains("localPos2"))
		{
			const auto& pos = _j.at("localPos2");
			_data.localPos2.x = pos.value("x", 0.0f);
			_data.localPos2.y = pos.value("y", 0.0f);
			_data.localPos2.z = pos.value("z", 0.0f);
		}
	}
}

inline void FromCsv(std::stringstream& _ss, StageImportData& _s)
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
