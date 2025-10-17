#pragma once

struct Vector2F;

struct Vector2
{
	int x;
	int y;

	//‰‰ŽZ
	Vector2 operator+(const Vector2 _value)const;
	void operator+=(const Vector2 _value);
	
	Vector2 operator-(const Vector2 _value)const;
	void operator-=(const Vector2 _value);
	
	Vector2 operator*(const int _value)const;
	void operator*=(const int _value);
	Vector2 operator*(const Vector2 _value)const;
	void operator*=(const Vector2 _value);

	Vector2 operator/(const int _value)const;
	void operator/=(const int _value);
	Vector2 operator/(const Vector2 _value)const;
	void operator/=(const Vector2 _value);

	//•ÏŠ·
	Vector2F ToVector2F(void);
};