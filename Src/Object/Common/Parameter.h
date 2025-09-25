#pragma once

class Parameter
{
public:

	//コンストラクタ
	Parameter(void);

	//デストラクタ
	~Parameter(void);

	//パラメーター関係
	int maxHealth_;		//最大体力
	int maxSpeed_;		//最高速度
	int acceleration_;	//加速度
	int attack_;		//攻撃力
	int defense_;		//防御力
	int charge_;		//チャージ速度

	//演算
	Parameter operator+(const Parameter _param);
	void operator+=(const Parameter _param);
};

