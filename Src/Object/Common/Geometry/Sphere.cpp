#include"../pch.h"
#include "../../../Common/Quaternion.h"
#include "../../../Utility/Utility.h"
#include"Model.h"
#include"Cube.h"
#include"Capsule.h"
#include"Line.h"
#include"Sphere.h"

//***************************************************
//球
//***************************************************

Sphere::Sphere(const VECTOR& _pos, const VECTOR& _movedPos, const float _radius)
	: Geometry(_pos, Quaternion()),
	movedPos_(_movedPos),
	radius_(_radius)
{
	std::memset(&hitInfo_, 0, sizeof(hitInfo_));
}

Sphere::Sphere(const Sphere& _copyBase)
	: Geometry(_copyBase.GetColPos(), Quaternion()),
	movedPos_(_copyBase.movedPos_)
{
	radius_ = _copyBase.GetRadius();
	std::memset(&hitInfo_, 0, sizeof(hitInfo_));
}

Sphere::~Sphere(void)
{
}

void Sphere::Draw(const int _color)
{
	DrawSphere3D(pos_, radius_, 10, _color, _color, false);
}

const bool Sphere::IsHit(Geometry& _geometry)
{
	return _geometry.IsHit(*this);
}

const bool Sphere::IsHit(Model& _model)
{
	return _model.IsHit(*this);
}

const bool Sphere::IsHit(Cube& _cube)
{
	// --- OBB情報 ---
	const auto& obb = _cube.GetObb();
	VECTOR center = VAdd(_cube.GetColPos(),
		VScale(VAdd(obb.vMin, obb.vMax), 0.5f)); // OBB中心
	VECTOR half = VScale(VSub(obb.vMax, obb.vMin), 0.5f); // 半サイズ

	// --- ローカル空間へ変換 ---
	VECTOR relPos = VSub(pos_, center);
	VECTOR velocity = VSub(movedPos_, pos_);
	VECTOR localP0 = VGet(VDot(relPos, obb.axis[0]),
		VDot(relPos, obb.axis[1]),
		VDot(relPos, obb.axis[2]));
	VECTOR localV = VGet(VDot(velocity, obb.axis[0]),
		VDot(velocity, obb.axis[1]),
		VDot(velocity, obb.axis[2]));

	// --- AABBスイープ ---
	VECTOR boxMin = VScale(half, -1.0f);
	VECTOR boxMax = half;
	VECTOR bmin = VSub(boxMin, VGet(radius_, radius_, radius_));
	VECTOR bmax = VAdd(boxMax, VGet(radius_, radius_, radius_));

	//スラブ法で衝突しているかを判定
	float tEnter = 0.0f;
	float tExit = 1.0f;

	for (int i = 0; i < 3; i++)
	{
		float start = (&localP0.x)[i];
		float dir = (&localV.x)[i];
		float minB = (&bmin.x)[i];
		float maxB = (&bmax.x)[i];

		if (fabs(dir) < 1e-6f)
		{
			if (start < minB || start > maxB)
				return false; // 外れている → 衝突なし
		}
		else
		{
			float t1 = (minB - start) / dir;
			float t2 = (maxB - start) / dir;
			if (t1 > t2) std::swap(t1, t2);

			tEnter = std::max(tEnter, t1);
			tExit = std::min(tExit, t2);

			if (tEnter > tExit)
				return false;
		}
	}

	// --- 衝突あり ---

	//衝突した瞬間を格納
	hitResult_.t = std::max(0.0f, tEnter);
	
	//Cube側も格納しておく
	HitResult cubeResult;
	cubeResult.t = hitResult_.t;

	VECTOR localHitPos = VAdd(localP0, VScale(localV, hitResult_.t));

	// --- 衝突法線 ---
	VECTOR localNormal = VGet(0, 0, 0);
	float eps = 1e-4f;
	for (int i = 0; i < 3; i++)
	{
		if (fabs((&localHitPos.x)[i] - (&bmin.x)[i]) < eps)
			(&localNormal.x)[i] = -1.0f;
		else if (fabs((&localHitPos.x)[i] - (&bmax.x)[i]) < eps)
			(&localNormal.x)[i] = 1.0f;
	}
	
	float dxMin = fabs(localHitPos.x - bmin.x);
	float dxMax = fabs(localHitPos.x - bmax.x);
	float dyMin = fabs(localHitPos.y - bmin.y);
	float dyMax = fabs(localHitPos.y - bmax.y);
	float dzMin = fabs(localHitPos.z - bmin.z);
	float dzMax = fabs(localHitPos.z - bmax.z);

	// 最小距離の軸が衝突面
	float minDist = dxMin;
	localNormal = VGet(-1, 0, 0);

	if (dxMax < minDist) { minDist = dxMax; localNormal = VGet(1, 0, 0); }
	if (dyMin < minDist) { minDist = dyMin; localNormal = VGet(0, -1, 0); }
	if (dyMax < minDist) { minDist = dyMax; localNormal = VGet(0, 1, 0); }
	if (dzMin < minDist) { minDist = dzMin; localNormal = VGet(0, 0, -1); }
	if (dzMax < minDist) { minDist = dzMax; localNormal = VGet(0, 0, 1); }

	//深度
	float depth = 0.0f;

	if (localNormal.x > 0) depth = bmax.x - localHitPos.x;
	else if (localNormal.x < 0) depth = localHitPos.x - bmin.x;
	else if (localNormal.y > 0) depth = bmax.y - localHitPos.y;
	else if (localNormal.y < 0) depth = localHitPos.y - bmin.y;
	else if (localNormal.z > 0) depth = bmax.z - localHitPos.z;
	else if (localNormal.z < 0) depth = localHitPos.z - bmin.z;

	// --- ワールド座標に戻す ---
	VECTOR hitWorld = center;
	hitWorld = VAdd(hitWorld,
		VAdd(VScale(obb.axis[0], localHitPos.x),
			VAdd(VScale(obb.axis[1], localHitPos.y),
				VScale(obb.axis[2], localHitPos.z))));

	VECTOR normalWorld = VGet(
		localNormal.x * obb.axis[0].x + localNormal.y * obb.axis[1].x + localNormal.z * obb.axis[2].x,
		localNormal.x * obb.axis[0].y + localNormal.y * obb.axis[1].y + localNormal.z * obb.axis[2].y,
		localNormal.x * obb.axis[0].z + localNormal.y * obb.axis[1].z + localNormal.z * obb.axis[2].z
	);
	normalWorld = VNorm(normalWorld);

	//情報の格納
	hitResult_.depth = depth;
	hitResult_.point = hitWorld;
	hitResult_.normal = normalWorld;

	//Cube側も渡す
	cubeResult.depth = depth;
	cubeResult.point = hitWorld;
	cubeResult.normal = VScale(normalWorld, -1.0f);

	return true;
}

const bool Sphere::IsHit(Sphere& _sphere)
{
	// 球体同士の衝突判定
	bool ret = false;

	// お互いの半径の合計
	float radius = GetRadius() + _sphere.GetRadius();

	// 座標の差からお互いの距離を取る
	VECTOR diff = VSub(_sphere.GetColPos(), GetColPos());

	// 三平方の定理で比較(SqrMagnitudeと同じ)
	float dis = (diff.x * diff.x) + (diff.y * diff.y) + (diff.z * diff.z);
	if (dis < (radius * radius))
	{
		ret = true;
	}

	return ret;
}

const bool Sphere::IsHit(Capsule& _capsule)
{
	//球体とカプセルの当たり判定
	bool ret = false;

	// カプセル球体の中心を繋ぐベクトル
	VECTOR cap1to2 = VSub(_capsule.GetPosDown(), _capsule.GetPosTop());

	// ベクトルを正規化
	VECTOR cap1to2ENor = VNorm(cap1to2);

	// カプセル繋ぎの単位ベクトルと、
	// そのベクトル元から球体へのベクトルの内積を取る
	float dot = VDot(cap1to2ENor, VSub(GetColPos(), _capsule.GetPosTop()));

	// 内積で求めた射影距離を使って、カプセル繋ぎ上の座標を取る
	VECTOR capRidePos = VAdd(_capsule.GetPosTop(), VScale(cap1to2ENor, dot));

	// カプセル繋ぎのベクトルの長さを取る
	float len = sqrt((cap1to2.x * cap1to2.x) + (cap1to2.y * cap1to2.y) + (cap1to2.z * cap1to2.z));

	// 球体がカプセル繋ぎ上にいるか判別するため、比率を取る
	float rate = dot / len;

	VECTOR centerPos = { 0.0f,0.0f,0.0f };

	// 球体の位置が３エリアに分割されたカプセル形状のどこにいるか判別
	if (rate > 0.0f && rate <= 1.0f)
	{
		// ①球体がカプセル繋ぎ上にいる
		centerPos = VAdd(_capsule.GetPosTop(), VScale(cap1to2ENor, dot));
	}
	else if (rate > 1.0f)
	{
		// ②球体がカプセルの終点側にいる
		centerPos = _capsule.GetPosDown();
	}
	else if (rate < 0.0f)
	{
		// ③球体がカプセルの始点側にいる
		centerPos = _capsule.GetPosTop();
	}
	else
	{
		// ここにきてはいけない
		return false;
	}

	// お互いの半径の合計
	float radius = GetRadius() + _capsule.GetRadius();

	// 座標の差からお互いの距離を取る
	VECTOR diff = VSub(centerPos, GetColPos());

	// 三平方の定理で比較(SqrMagnitudeと同じ)
	float dis = (diff.x * diff.x) + (diff.y * diff.y) + (diff.z * diff.z);
	if (dis < (radius * radius))
	{
		ret = true;
	}

	return ret;
}

const bool Sphere::IsHit(Line& _line)
{
	//線のベクトル
	VECTOR d = VSub(_line.GetPosPoint2(), _line.GetPosPoint1());

	//線の先端から球体の中心まで
	VECTOR m = VSub(GetColPos(), _line.GetPosPoint1());

	float t = VDot(m, d) / VSquareSize(d);
	t = std::max<float>(0.0f, std::min<float>(1.0f, t));  // 線分内に制限

	VECTOR closestPoint = VAdd(_line.GetPosPoint1(), VScale(d, t));
	VECTOR diff = VSub(closestPoint, GetColPos());

	return VSquareSize(diff) <= std::pow(GetRadius(), 2.0);
}

void Sphere::HitAfter(void)
{
	if (hitInfo_.HitNum > 0 && hitInfo_.Dim != nullptr)
	{
		//当たり判定情報の解放
		MV1CollResultPolyDimTerminate(hitInfo_);

		//再初期化
		std::memset(&hitInfo_, 0, sizeof(hitInfo_));
	}
}
