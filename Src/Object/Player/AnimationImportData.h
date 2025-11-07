#pragma once
#include"../Lib/nlohmann/json.hpp"
#include<iostream>
#include<unordered_map>
#include<string>

struct AnimationImportData
{
	//名前
	std::string name;

	//アニメーション
	std::unordered_map<std::string, int> animation;
};

//Json呼び出し
inline void FromJson(const nlohmann::json& _j, AnimationImportData& _data)
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

	//座標
	if (_j.contains("animation"))
	{
		std::string animName;
		int animNum;

		for (const auto& anim : _j["animation"])
		{
			animName = anim.value("animName", "");
			animNum = anim.value("number", -1);
			_data.animation.emplace(animName, animNum);
		}
	}
}

inline void FromCsv(std::stringstream& _ss, AnimationImportData& _s)
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
