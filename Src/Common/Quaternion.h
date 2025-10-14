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
	
	/// @brief コンストラクタ
	/// @param _rad ラジアン角
	Quaternion(const VECTOR& _rad);
	
	/// @brief コンストラクタ
	/// @param _ww スカラー値
	/// @param _wx Xベクトル
	/// @param _wy Yベクトル
	/// @param _wz Zベクトル
	Quaternion(double _ww, double _wx, double _wy, double _wz);

	//デストラクタ
	~Quaternion(void);

	/// @brief オイラー角からクォータニオンへ変換
	/// @param _rad ラジアン角
	/// @return オイラー角をもとにしたクォータニオン
	static Quaternion Euler(const VECTOR& _rad);
	
	/// @brief オイラー角からクォータニオンへ変換
	/// @param _radX Xラジアン角
	/// @param _radY Yラジアン角
	/// @param _radZ Zラジアン角
	/// @return オイラー角をもとにしたクォータニオン
	static Quaternion Euler(double _radX, double _radY, double _radZ);

	/// @brief クォータニオンの合成
	/// @param _q1 クォータニオン1
	/// @param _q2 クォータニオン2
	/// @return q1とq2の合成クォータニオン
	static Quaternion Mult(const Quaternion& _q1, const Quaternion& _q2);
	
	/// @brief クォータニオンの合成
	/// @param _q クォータニオン
	/// @return 合成クォータニオン
	Quaternion Mult(const Quaternion& _q) const;

	/// @brief 指定軸を指定角分、回転させる
	/// @param _rad ラジアン角
	/// @param _axis 回転軸
	/// @return 回転後クォータニオン
	static Quaternion AngleAxis(double _rad, VECTOR _axis);

	/// @brief 座標を回転させる
	/// @param _q 回転用クォータニオン
	/// @param _pos 回転する座標
	/// @return 回転後座標
	static VECTOR PosAxis(const Quaternion& _q, VECTOR _pos);

	/// @brief 座標を回転させる
	/// @param _pos 回転する座標
	/// @return 回転後座標
	VECTOR PosAxis(VECTOR _pos) const;

	/// @brief クォータニオンからオイラー角へ変換
	/// @param _q クォータニオン
	/// @return クォータニオンを元にしたオイラー角
	static VECTOR ToEuler(const Quaternion& _q);

	/// @brief クォータニオンからオイラー角へ変換
	/// @param  
	/// @return クォータニオンを元にしたオイラー角
	VECTOR ToEuler(void) const;

	/// @brief クォータニオンから行列へ変換
	/// @param _q クォータニオン
	/// @return クォータニオンを元にした行列
	static MATRIX ToMatrix(const Quaternion& _q);

	/// @brief クォータニオンから行列へ変換
	/// @param  
	/// @return クォータニオンを元にした行列
	MATRIX ToMatrix(void) const;

	/// @brief ベクトルからクォータニオンに変換
	/// @param _dir 方向ベクトル
	/// @return ベクトルを元にしたクォータニオン
	static Quaternion LookRotation(VECTOR _dir);

	/// @brief ベクトルからクォータニオンに変換
	/// @param _dir 方向ベクトル
	/// @param _up 上方向ベクトル
	/// @return ベクトルを元にしたクォータニオン
	static Quaternion LookRotation(VECTOR _dir, VECTOR _up);

	/// @brief 行列からクォータニオンに変換
	/// @param _mat 行列
	/// @return 行列をもとにしたクォータニオン
	static Quaternion GetRotation(MATRIX _mat);

	/// @brief 前方向ベクトルの取得
	/// @param  
	/// @return 前方向ベクトル
	VECTOR GetForward(void) const;

	/// @brief 後方向ベクトルの取得
	/// @param  
	/// @return 後方向ベクトル
	VECTOR GetBack(void) const;

	/// @brief 右方向ベクトルの取得
	/// @param  
	/// @return 右方向ベクトル
	VECTOR GetRight(void) const;

	/// @brief 左方向ベクトルの取得
	/// @param  
	/// @return 左方向ベクトル
	VECTOR GetLeft(void) const;
	
	/// @brief 上方向ベクトルの取得
	/// @param  
	/// @return 上方向ベクトル
	VECTOR GetUp(void) const;

	/// @brief 下方向ベクトルの取得
	/// @param  
	/// @return 下方向ベクトル
	VECTOR GetDown(void) const;

	/// @brief 内積
	/// @param _q1 クォータニオン1
	/// @param _q2 クォータニオン2
	/// @return q1とq2の内積
	static double Dot(const Quaternion& _q1, const Quaternion& _q2);

	/// @brief 内積
	/// @param _b クォータニオン
	/// @return 内積
	double Dot(const Quaternion& _b) const;

	/// @brief 正規化
	/// @param _q クォータニオン
	/// @return 単位クォータニオン
	static Quaternion Normalize(const Quaternion& _q);

	/// @brief 正規化
	/// @param  
	/// @return 単位クォータニオン
	Quaternion Normalized(void) const;

	//正規化
	void Normalize(void);

	//逆クォータニオン
	Quaternion Inverse(void) const;

	/// @brief 球面補間
	/// @param _from 元クォータニオン
	/// @param _to 回転後クォータニオン
	/// @param _t 補間比率(0.0～1.0)
	/// @return 比率に基づいた回転後クォータニオン
	static Quaternion Slerp(Quaternion _from, Quaternion _to, double _t);

	/// @brief ２つのベクトル間の回転量を取得する
	/// @param _fromDir 元ベクトル
	/// @param _toDir 回転後ベクトル
	/// @return 回転量クォータニオン
	static Quaternion FromToRotation(VECTOR _fromDir, VECTOR _toDir);
	
	/// @brief ２つのベクトル間の回転量を取得する
	/// @param _from 元クォータニオン
	/// @param _to 回転後クォータニオン
	/// @param _maxDegreesDelta 進める角度
	/// @return 角度に基づいた回転量クォータニオン
	static Quaternion RotateTowards(const Quaternion& _from, const Quaternion& _to, float _maxDegreesDelta);
	
	/// @brief ２つのベクトル間の回転量を取得する
	/// @param _q1 クォータニオン1
	/// @param _q2 クォータニオン2
	/// @return q1とq2の回転角度(デグリー角)
	static double Angle(const Quaternion& _q1, const Quaternion& _q2);
	
	/// @brief 球面補間(範囲制限なし)
	/// @param _a 元クォータニオン
	/// @param _b 回転後クォータニオン
	/// @param _t 補間比率
	/// @return 比率に基づいた回転後クォータニオン
	static Quaternion SlerpUnclamped(Quaternion _a, Quaternion _b, float _t);
	
	//単位クォータニオン
	static Quaternion Identity(void);

	//クォータニオンの長さ
	double Length(void) const;

	//クォータニオンの長さ(二乗)
	double LengthSquared(void) const;
	
	//VECTOR変換
	VECTOR xyz(void) const;

	/// @brief 対象方向の回転
	/// @param _angle 参照回転
	/// @param _axis 参照回転軸
	void ToAngleAxis(float* _angle, VECTOR* _axis);

private:

	// 基本ベクトルを取得
	VECTOR GetDir(VECTOR _dir) const;

	Quaternion operator*(float& _rhs);
	const Quaternion operator*(const float& _rhs);
	Quaternion operator+(Quaternion& _rhs);
	const Quaternion operator+(const Quaternion& _rhs);
	
};
