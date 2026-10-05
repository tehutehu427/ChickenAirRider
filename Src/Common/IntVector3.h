#pragma once

//整数の3次元ベクトル
struct IntVector3
{
	int x;
	int y;
	int z;

	//演算
	const IntVector3 operator+(const IntVector3 _value)const;
	void operator+=(const IntVector3 _value);
	const IntVector3 operator-(const IntVector3 _value)const;
	void operator-=(const IntVector3 _value);
	const IntVector3 operator*(const int _value)const;
	void operator*=(const int _value);
	const IntVector3 operator/(const int _value)const;
	void operator/=(const int _value);

	//比較
	const bool operator==(const IntVector3 _value);
	const bool operator<(const IntVector3 _value);
	const bool operator<=(const IntVector3 _value);
	const bool operator>(const IntVector3 _value);
	const bool operator>=(const IntVector3 _value);
};

//IntVector3の初期化用
static constexpr IntVector3 INT_VECTOR_ZERO = { 0,0,0 };
static constexpr IntVector3 INT_VECTOR_ONE = { 1,1,1 };