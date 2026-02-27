#include <cmath>
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

float Vector2F::Length(void) const
{
	return std::sqrtf((x * x) + (y * y));
}

void Vector2F::Normalize(void)
{
	float length = Length();

	if (length == 0.0f)
	{
		x = 0.0f;
		y = 0.0f;
		return;
	}

	x /= length;
	y /= length;
}

Vector2F Vector2F::Normalized(void) const
{
	float length = Length();
	return { x / length,y / length };
}
