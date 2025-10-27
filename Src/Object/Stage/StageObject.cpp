#include "../Utility/Utility.h"
#include "../Manager/System/ResourceManager.h"
#include "../Common/Geometry/Cube.h"
#include "StageObject.h"

StageObject::StageObject(const int _modelId, const VECTOR _pos, const VECTOR _scl, const Quaternion _quaRot)
{
	trans_.modelId = _modelId;
	trans_.pos = _pos;
	trans_.scl = _scl;
	trans_.quaRot = _quaRot;

	color_ = 0;
}

StageObject::~StageObject(void)
{
}

void StageObject::Load(void)
{

}

void StageObject::Init(void)
{
	//ÉRÉâÉCÉ_ê∂ê¨
	std::unique_ptr<Geometry> geo = std::make_unique<Cube>(trans_.pos, trans_.quaRot, STAGE_SIZE);
	MakeCollider({ Collider::TAG::STAGE }, std::move(geo));
}

void StageObject::Update(void)
{
	trans_.Update();
}

void StageObject::Draw(void)
{
	colParam_[0].geometry_->Draw(color_);

	DrawSphere3D(VGet(0.0f, 0.0f, 1000.0f), 100.0f, 20, color_, color_, true);

	MV1DrawModel(trans_.modelId);

	color_ = 0;
}

void StageObject::OnHit(const std::weak_ptr<Collider> _hitCol)
{
	color_ = 0xff0000;
}
