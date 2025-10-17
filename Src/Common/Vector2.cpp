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

Vector2F Vector2::ToVector2F(void)
{
	return { static_cast<float>(x),static_cast<float>(y) };
}