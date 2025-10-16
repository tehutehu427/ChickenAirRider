#pragma once

#include<DxLib.h>
#include"../../../Common/Quaternion.h"

class Model;
class Cube;
class Sphere;
class Capsule;
class Line;

class Geometry
{
public:

	//デストラクタ
	virtual~Geometry(void) = 0;

	//描画
	virtual void Draw(const int _color = NORMAL_COLOR) = 0;

	//各種当たり判定
	virtual const bool IsHit(Geometry& _geometry) = 0;
	virtual const bool IsHit(Model& _model) = 0;
	virtual const bool IsHit(Cube& _cube) = 0;
	virtual const bool IsHit(Sphere& _sphere) = 0;
	virtual const bool IsHit(Capsule& _capsule) = 0;
	virtual const bool IsHit(Line& _line) = 0;
	
	//ヒット後の処理
	virtual void HitAfter(void);

	//親情報を返す
	inline const VECTOR& GetColPos(void)const { return pos_; }
	inline const Quaternion& GetColRot(void)const { return quaRot_; }

protected:

	//通常色
	static constexpr int NORMAL_COLOR = 0xffffff;

	/// @brief コンストラクタ(外部で作る必要のない基底なのでprotected)
	/// @param _pos 追従する親の座標
	/// @param _rot 追従する親の回転
	Geometry(const VECTOR& _pos, const Quaternion& _rot);

	// 相対座標を回転させてワールド座標で取得する
	const VECTOR GetRotPos(const VECTOR& _localPos) const;

	const VECTOR& pos_;			//親の座標
	const Quaternion& quaRot_;	//親の回転
};