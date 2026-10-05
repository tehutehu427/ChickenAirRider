#pragma once
#include <DxLib.h>
#include <iostream>
#include <algorithm>
class Quaternion
{

public:
	
	//最小の数
	static constexpr float kEpsilonNormalSqrt = 1e-15F;

	//回転要素
	double w;	//スカラー値
	double x;	//Xベクトル
	double y;	//Yベクトル
	double z;	//Zベクトル

	//コンストラクタ
	Quaternion(void);
	
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="_rad">ラジアン角</param>
	Quaternion(const VECTOR& _rad);
	
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="_ww">スカラー値</param>
	/// <param name="_wx">Xベクトル</param>
	/// <param name="_wy">Yベクトル</param>
	/// <param name="_wz">Zベクトル</param>
	Quaternion(double _ww, double _wx, double _wy, double _wz);

	//デストラクタ
	~Quaternion(void);

	/// <summary>
	/// オイラー角からクォータニオンへ変換
	/// </summary>
	/// <param name="_rad">ラジアン角</param>
	/// <returns>オイラー角をもとにしたクォータニオン</returns>
	static Quaternion Euler(const VECTOR& _rad);
	
	/// <summary>
	/// オイラー角からクォータニオンへ変換
	/// </summary>
	/// <param name="_radX">Xラジアン角</param>
	/// <param name="_radY">Yラジアン角</param>
	/// <param name="_radZ">Zラジアン角</param>
	/// <returns>オイラー角をもとにしたクォータニオン</returns>
	static Quaternion Euler(double _radX, double _radY, double _radZ);
	
	/// <summary>
	/// クォータニオンの合成
	/// </summary>
	/// <param name="_q1">クォータニオン1</param>
	/// <param name="_q2">クォータニオン2</param>
	/// <returns>q1とq2の合成クォータニオン</returns>
	static Quaternion Mult(const Quaternion& _q1, const Quaternion& _q2);
	
	/// <summary>
	/// クォータニオンの合成
	/// </summary>
	/// <param name="_q">クォータニオン</param>
	/// <returns>合成クォータニオン</returns>
	Quaternion Mult(const Quaternion& _q) const;

	/// <summary>
	/// 指定軸を指定角分、回転させる
	/// </summary>
	/// <param name="_rad">ラジアン角</param>
	/// <param name="_axis">回転軸</param>
	/// <returns>回転後クォータニオン</returns>
	static Quaternion AngleAxis(double _rad, VECTOR _axis);

	/// <summary>
	/// 座標を回転させる
	/// </summary>
	/// <param name="_q">回転用クォータニオン</param>
	/// <param name="_pos">回転する座標</param>
	/// <returns>回転後座標</returns>
	static VECTOR PosAxis(const Quaternion& _q, VECTOR _pos);

	/// <summary>
	/// 座標を回転させる
	/// </summary>
	/// <param name="_pos">回転する座標</param>
	/// <returns>回転後座標</returns>
	VECTOR PosAxis(VECTOR _pos) const;

	/// <summary>
	/// クォータニオンからオイラー角へ変換
	/// </summary>
	/// <param name="_q">クォータニオン</param>
	/// <returns>クォータニオンを元にしたオイラー角</returns>
	static VECTOR ToEuler(const Quaternion& _q);

	//クォータニオンからオイラー角へ変換
	VECTOR ToEuler(void) const;

	/// <summary>
	/// クォータニオンから行列へ変換
	/// </summary>
	/// <param name="_q">クォータニオン</param>
	/// <returns>クォータニオンを元にした行列</returns>
	static MATRIX ToMatrix(const Quaternion& _q);

	//クォータニオンから行列へ変換
	MATRIX ToMatrix(void) const;

	/// <summary>
	/// ベクトルからクォータニオンに変換
	/// </summary>
	/// <param name="_dir">方向ベクトル</param>
	/// <returns>ベクトルを元にしたクォータニオン</returns>
	static Quaternion LookRotation(VECTOR _dir);

	/// <summary>
	/// ベクトルからクォータニオンに変換
	/// </summary>
	/// <param name="_dir">方向ベクトル</param>
	/// <param name="_up">上方向ベクトル</param>
	/// <returns>ベクトルを元にしたクォータニオン</returns>
	static Quaternion LookRotation(VECTOR _dir, VECTOR _up);

	/// <summary>
	/// 行列からクォータニオンに変換
	/// </summary>
	/// <param name="_mat">行列</param>
	/// <returns>行列をもとにしたクォータニオン</returns>
	static Quaternion GetRotation(MATRIX _mat);

	//前方向ベクトルの取得
	VECTOR GetForward(void) const;

	//後方向ベクトルの取得
	VECTOR GetBack(void) const;

	//右方向ベクトルの取得
	VECTOR GetRight(void) const;

	//左方向ベクトルの取得
	VECTOR GetLeft(void) const;
	
	//上方向ベクトルの取得
	VECTOR GetUp(void) const;

	//下方向ベクトルの取得
	VECTOR GetDown(void) const;

	/// <summary>
	/// 内積
	/// </summary>
	/// <param name="_q1">クォータニオン1</param>
	/// <param name="_q2">クォータニオン2</param>
	/// <returns>q1とq2の内積</returns>
	static double Dot(const Quaternion& _q1, const Quaternion& _q2);

	/// <summary>
	/// 内積
	/// </summary>
	/// <param name="_b">相手のクォータニオン</param>
	/// <returns>内積</returns>
	double Dot(const Quaternion& _b) const;

	/// <summary>
	/// 正規化
	/// </summary>
	/// <param name="_q">クォータニオン</param>
	/// <returns>単位クォータニオン</returns>
	static Quaternion Normalize(const Quaternion& _q);

	//正規化
	Quaternion Normalized(void) const;

	//正規化
	void Normalize(void);

	//逆クォータニオン
	Quaternion Inverse(void) const;

	/// <summary>
	/// 球面補間
	/// </summary>
	/// <param name="_from">元クォータニオン</param>
	/// <param name="_to">回転後クォータニオン</param>
	/// <param name="_t">補間比率(0.0～1.0)</param>
	/// <returns>比率に基づいた回転後クォータニオン</returns>
	static Quaternion Slerp(Quaternion _from, Quaternion _to, double _t);

	/// <summary>
	/// ２つのベクトル間の回転量を取得する
	/// </summary>
	/// <param name="_fromDir">元ベクトル</param>
	/// <param name="_toDir">回転後ベクトル</param>
	/// <returns>回転量クォータニオン</returns>
	static Quaternion FromToRotation(VECTOR _fromDir, VECTOR _toDir);
	
	/// <summary>
	/// ２つのベクトル間の回転量を取得する
	/// </summary>
	/// <param name="_from">元クォータニオン</param>
	/// <param name="_to">回転後クォータニオン</param>
	/// <param name="_maxDegreesDelta">進める角度</param>
	/// <returns>角度に基づいた回転量クォータニオン</returns>
	static Quaternion RotateTowards(const Quaternion& _from, const Quaternion& _to, float _maxDegreesDelta);
	
	/// <summary>
	/// ２つのベクトル間の回転量を取得する
	/// </summary>
	/// <param name="_q1">クォータニオン1</param>
	/// <param name="_q2">クォータニオン2</param>
	/// <returns>q1とq2の回転角度(デグリー角)</returns>
	static double Angle(const Quaternion& _q1, const Quaternion& _q2);
	
	/// <summary>
	/// 球面補間(範囲制限なし)
	/// </summary>
	/// <param name="_a">元クォータニオン</param>
	/// <param name="_b">回転後クォータニオン</param>
	/// <param name="_t">補間比率</param>
	/// <returns>比率に基づいた回転後クォータニオン</returns>
	static Quaternion SlerpUnclamped(Quaternion _a, Quaternion _b, float _t);
	
	//単位クォータニオン
	static Quaternion Identity(void);

	//クォータニオンの長さ
	double Length(void) const;

	//クォータニオンの長さ(二乗)
	double LengthSquared(void) const;
	
	//VECTOR変換
	VECTOR xyz(void) const;

	/// <summary>
	/// 対象方向の回転
	/// </summary>
	/// <param name="_angle">参照回転</param>
	/// <param name="_axis">参照回転軸</param>
	void ToAngleAxis(float* _angle, VECTOR* _axis);

private:

	// 基本ベクトルを取得
	VECTOR GetDir(VECTOR _dir) const;

	//演算子
	Quaternion operator*(float& _rhs);
	const Quaternion operator*(const float& _rhs);
	Quaternion operator+(Quaternion& _rhs);
	const Quaternion operator+(const Quaternion& _rhs);	
};
