#pragma once

struct IntVector3
{
	int x;
	int y;
	int z;

	//ââéZ
	const IntVector3 operator+(const IntVector3 _value)const;
	void operator+=(const IntVector3 _value);
	const IntVector3 operator-(const IntVector3 _value)const;
	void operator-=(const IntVector3 _value);
	const IntVector3 operator*(const int _value)const;
	void operator*=(const int _value);
	const IntVector3 operator/(const int _value)const;
	void operator/=(const int _value);

	//î‰är
	const bool operator==(const IntVector3 _value);
	const bool operator<(const IntVector3 _value);
	const bool operator<=(const IntVector3 _value);
	const bool operator>(const IntVector3 _value);
	const bool operator>=(const IntVector3 _value);
};

//IntVector3ÇÃèâä˙âªóp
static constexpr IntVector3 INT_VECTOR_ZERO = { 0,0,0 };
static constexpr IntVector3 INT_VECTOR_ONE = { 1,1,1 };