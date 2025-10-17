#pragma once

struct Vector2;

struct Vector2F
{
	float x;
	float y;

	//‰‰ŽZ
	Vector2F operator+(const Vector2F _value)const;
	void operator+=(const Vector2F _value);

	Vector2F operator-(const Vector2F _value)const;
	void operator-=(const Vector2F _value);

	Vector2F operator*(const float _value)const;
	void operator*=(const float _value);
	Vector2F operator*(const Vector2F _value)const;
	void operator*=(const Vector2F _value);

	Vector2F operator/(const float _value)const;
	void operator/=(const float _value);
	Vector2F operator/(const Vector2F _value)const;
	void operator/=(const Vector2F _value);

	//•ÏŠ·
	Vector2 ToVector2(void);
};
