#pragma once

//攻撃情報
struct AttackData
{
	//攻撃属性
	enum class ATTACK_ELEMENT
	{
		NORMAL	//通常
		, ITEM	//アイテム
		, MAX
	};

	//威力
	float power = 0.0f;

	//連続ヒットするか
	bool isMultiHit = false;

	//連続ヒット間隔
	float hitInterval = -1.0f;

	//攻撃属性
	ATTACK_ELEMENT element = ATTACK_ELEMENT::NORMAL;
};