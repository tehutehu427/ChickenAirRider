#pragma once

struct Vector2;

struct Vector2F
{
	float x = 0.0f;
	float y = 0.0f;

	//ââéZ
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

	//ïœä∑
	Vector2 ToVector2(void);

	//ëÂÇ´Ç≥
	float Length(void)const;

	//ê≥ãKâª
	void Normalize(void);
	Vector2F Normalized(void)const;
};
