#include "Vector2.h"
#include "Vector2F.h"

Vector2 Vector2::operator+(const Vector2 _value) const
{
	return { x + _value.x,y + _value.y };
}

void Vector2::operator+=(const Vector2 _value)
{
	x += _value.x;
	y += _value.y;
}

Vector2 Vector2::operator-(const Vector2 _value) const
{
	return { x - _value.x, y - _value.y };
}

void Vector2::operator-=(const Vector2 _value)
{
	x -= _value.x;
	y -= _value.y;
}

Vector2 Vector2::operator*(const int _value) const
{
	return { x * _value , y * _value };
}

void Vector2::operator*=(const int _value)
{
	x *= _value;
	y *= _value;
}

Vector2 Vector2::operator*(const Vector2 _value) const
{
	return { x * _value.x, y * _value.y };
}

void Vector2::operator*=(const Vector2 _value)
{
	x *= _value.x;
	y *= _value.y;
}

Vector2 Vector2::operator*(const float _value) const
{
	float newX = static_cast<float>(x) * _value;
	float newY = static_cast<float>(y) * _value;

	return { static_cast<int>(newX), static_cast<int>(newY) };
}

void Vector2::operator*=(const float _value)
{
	float newX = static_cast<float>(x) * _value;
	float newY = static_cast<float>(y) * _value;

	x = static_cast<int>(newX);
	y = static_cast<int>(newY);
}

Vector2 Vector2::operator/(const int _value) const
{
	return { x / _value, y / _value };
}

void Vector2::operator/=(const int _value)
{
	x /= _value;
	y /= _value;
}

Vector2 Vector2::operator/(const Vector2 _value) const
{
	return { x / _value.x,y / _value.y };
}

void Vector2::operator/=(const Vector2 _value)
{
	x /= _value.x;
	y /= _value.y;
}

Vector2 Vector2::operator/(const float _value) const
{
	float newX = static_cast<float>(x) / _value;
	float newY = static_cast<float>(y) / _value;

	return {static_cast<int>(newX), static_cast<int>(newY)};
}

void Vector2::operator/=(const float _value)
{
	float newX = static_cast<float>(x) / _value;
	float newY = static_cast<float>(y) / _value;

	x = static_cast<int>(newX);
	y = static_cast<int>(newY);
}

Vector2F Vector2::ToVector2F(void)
{
	return { static_cast<float>(x),static_cast<float>(y) };
}