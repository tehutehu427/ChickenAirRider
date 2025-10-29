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

	//ヒット情報
	struct HitResult
	{
		bool hit = false;
		float t = 1.0f;        // 0～1：衝突した割合（移動途中の位置）
		VECTOR point = { 0,0,0 };
		VECTOR normal = { 0,0,0 };
		float depth = 0.0f;
	};

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

	//親の座標を返す
	inline const VECTOR& GetColPos(void)const { return pos_; }

	//親の移動前座標を返す
	inline const VECTOR& GetColPrePos(void)const { return prePos_; }
	
	//親の回転を返す
	inline const Quaternion& GetColRot(void)const { return quaRot_; }

	//当たった法線方向の取得
	inline const VECTOR GetHitNormal(void)const { return hitResult_.normal; }

	//当たった法線方向の設定
	inline void SetHitNormal(const VECTOR _hitNormal) { hitResult_.normal = _hitNormal; }

	//めり込み深度の取得
	inline const float GetHitDepth(void)const { return hitResult_.depth; }

	//めり込み深度の設定
	inline void SetHitDepth(const float _depth) { hitResult_.depth = _depth; }

	// 相対座標を回転させてワールド座標で取得する
	const VECTOR GetRotPos(const VECTOR& _localPos) const;
	
	// 相対座標を逆回転させてワールド座標で取得する
	const VECTOR GetInvRotPos(const VECTOR& _localPos) const;

protected:

	//通常色
	static constexpr int NORMAL_COLOR = 0xffffff;

	//親情報
	const VECTOR& pos_;			//親の座標
	const VECTOR& prePos_;			//親の移動前座標
	const Quaternion& quaRot_;	//親の回転

	//当たった情報
	HitResult hitResult_;

	/// @brief コンストラクタ(外部で作る必要のない基底なのでprotected)
	/// @param _pos 追従する親の座標
	/// @param _prePos 追従する親の移動前座標(移動しないなら座標と同じ)
	/// @param _rot 追従する親の回転
	Geometry(const VECTOR& _pos, const VECTOR& _prePos,const Quaternion& _rot);
};