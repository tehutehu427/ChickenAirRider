#include "../Utility/Utility.h"
#include "../Manager/System/ResourceManager.h"
#include "../Common/Geometry/Sphere.h"
#include "../Common/Geometry/Capsule.h"
#include "../Common/Geometry/Cube.h"
#include "../Common/Geometry/Line.h"
#include "../Common/Geometry/Model.h"
#include "StageObject.h"

StageObject::StageObject(const ImportData& _data,const int _modelId, const std::set<Collider::TAG> _tags)
{
	trans_.modelId = _modelId;
	trans_.pos = _data.position;
	trans_.scl = _data.scale;
	trans_.quaRot = _data.quaternion;

	data_ = _data;

	createGeo_["sphere"] = [this](void) { return MakeSphere(); };
	createGeo_["capsule"] = [this](void) { return MakeCapsule(); };
	createGeo_["cube"] = [this](void) { return MakeCube(); };
	createGeo_["line"] = [this](void) { return MakeLine(); };
	createGeo_["model"] = [this](void) { return MakeModel(); };
	
	//ÉRÉâÉCÉ_ê∂ê¨
	std::unique_ptr<Geometry> geo = createGeo_[data_.geometry]();
	MakeCollider(_tags, std::move(geo));

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

std::unique_ptr<Geometry> StageObject::MakeSphere(void)
{
	return std::make_unique<Sphere>(trans_.pos, data_.radius);
}

std::unique_ptr<Geometry> StageObject::MakeCapsule(void)
{
	return std::make_unique<Capsule>(trans_.pos, trans_.quaRot, data_.localPos1, data_.localPos2, data_.radius);
}

std::unique_ptr<Geometry> StageObject::MakeCube(void)
{
	return std::make_unique<Cube>(trans_.pos, trans_.quaRot, data_.localPos1, data_.localPos2);
}

std::unique_ptr<Geometry> StageObject::MakeLine(void)
{
	return std::make_unique<Line>(trans_.pos, trans_.quaRot, data_.localPos1, data_.localPos2);
}

std::unique_ptr<Geometry> StageObject::MakeModel(void)
{
	return std::make_unique<Model>(trans_.pos, trans_.quaRot, trans_.modelId);
}
