#pragma once
class Vector2
{
public:

	int x;
	int y;

	// コンストラクタ
	Vector2(void);

	// コンストラクタ
	Vector2(int vX, int vY);

	// デストラクタ
	~Vector2(void);

	/// @brief Vector2同士の足し算
	/// @param _value1 加算する値
	/// @param _value2 加算する値
	/// @return 加算した値
	static Vector2 AddVector2(const Vector2& _value1, const Vector2& _value2);

	/// @brief Vector2同士の引き算
	/// @param _value1 引かれる値
	/// @param _value2 引く値
	/// @return 減算した値
	static Vector2 SubVector2(const Vector2& _value1, const Vector2& _value2);

};

