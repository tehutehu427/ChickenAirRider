#include"../pch.h"
#include"../../Manager/System/ResourceManager.h"
#include "../Object/Common/Geometry/Sphere.h"
#include "Machine.h"

Machine::Machine(const int _modelId, const float _radius)
{
	trans_ = Transform();
	trans_.modelId = _modelId;
	radius_ = _radius;
	health_ = 0.0f;
}

Machine::~Machine(void)
{
}

void Machine::Load(void)
{
	//キャラクターのステータスを外部から持ってくる
	unitParam_.fixedMaxSpeed_ = PARAM_NORMAL;
	unitParam_.fixedAcceleration_ = PARAM_NORMAL;
	unitParam_.fixedTurning_ = PARAM_NORMAL;
	unitParam_.fixedCharge_ = PARAM_NORMAL;
	unitParam_.fixedFlight_ = PARAM_NORMAL;
	unitParam_.fixedWeight_ = PARAM_NORMAL;
	unitParam_.fixedAttack_ = PARAM_NORMAL;
	unitParam_.fixedDefence_ = PARAM_NORMAL;
	unitParam_.fixedMaxHealth_ = PARAM_NORMAL;

	unitParam_.affectMaxSpeed_ = 0.5f;
	unitParam_.affectAcceleration_ = 0.5f;
	unitParam_.affectTurning_ = 0.5f;
	unitParam_.affectCharge_ = 0.5f;
	unitParam_.affectFlight_ = 0.5f;
	unitParam_.affectWeight_ = 0.5f;
	unitParam_.affectAttack_ = 0.5f;
	unitParam_.affectDefence_ = 0.5f;
	unitParam_.affectMaxHealth_ = 0.5f;

	unitParam_.chargeBraking_ = 0.2f;
	unitParam_.chargeCapacity_ = 100.0f;
	unitParam_.chargeDamp_ = 1.0f;
	unitParam_.boostRate_ = 0.2f;
	unitParam_.boostPower_ = 6;

	//モデル
	trans_.scl = MODEL_SIZE;
	trans_.localPos.y -= 20.0f;
}

void Machine::Init(void)
{
}

void Machine::Update(void)
{
	trans_.Update();
}

void Machine::Draw(void)
{
	MV1DrawModel(trans_.modelId);

	for (const auto& col : collider_)
	{
		col->GetGeometry().Draw();
	}
}

void Machine::OnHit(const std::weak_ptr<Collider> _hitCol)
{
}

void Machine::CreateCol(void)
{
	//コライダ生成
	std::unique_ptr<Geometry> geo = std::make_unique<Sphere>(trans_.pos, trans_.pos, radius_);
	MakeCollider(Collider::TAG::MACHINE, std::move(geo), { Collider::TAG::MACHINE_RIDE });

	//乗り判定コライダ
	geo = std::make_unique<Sphere>(trans_.pos, trans_.pos, RIDE_COL);
	MakeCollider(Collider::TAG::MACHINE_RIDE, std::move(geo), { Collider::TAG::MACHINE,Collider::TAG::MACHINE_RIDE });
}

void Machine::DeleteCol(void)
{
	//コライダ削除
	DeleteAllCollider();
}

void Machine::SetScale(const VECTOR& _scale)
{
	VECTOR scale = _scale;
	scale.x = scale.x * MODEL_SIZE.x;
	scale.y = scale.y * MODEL_SIZE.y;
	scale.z = scale.z * MODEL_SIZE.z;

	trans_.scl = scale;
}
