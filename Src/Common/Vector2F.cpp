#include "Vector2F.h"
#include "Vector2.h"

Vector2F Vector2F::operator+(const Vector2F _value) const
{
	return { x + _value.x,y + _value.y };
}

void Vector2F::operator+=(const Vector2F _value)
{
	x += _value.x;
	y += _value.y;
}

Vector2F Vector2F::operator-(const Vector2F _value) const
{
	return { x - _value.x, y - _value.y };
}

void Vector2F::operator-=(const Vector2F _value)
{
	x -= _value.x;
	y -= _value.y;
}

Vector2F Vector2F::operator*(const float _value) const
{
	return { x * _value , y * _value };
}

void Vector2F::operator*=(const float _value)
{
	x *= _value;
	y *= _value;
}

Vector2F Vector2F::operator*(const Vector2F _value) const
{
	return { x * _value.x, y * _value.y };
}

void Vector2F::operator*=(const Vector2F _value)
{
	x *= _value.x;
	y *= _value.y;
}

Vector2F Vector2F::operator/(const float _value) const
{
	return { x / _value, y / _value };
}

void Vector2F::operator/=(const float _value)
{
	x /= _value;
	y /= _value;
}

Vector2F Vector2F::operator/(const Vector2F _value) const
{
	return { x / _value.x,y / _value.y };
}

void Vector2F::operator/=(const Vector2F _value)
{
	x /= _value.x;
	y /= _value.y;
}

Vector2 Vector2F::ToVector2(void)
{
	return { static_cast<int>(x),static_cast<int>(y) };
}