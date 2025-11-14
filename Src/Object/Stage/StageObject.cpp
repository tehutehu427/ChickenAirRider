#include "../Utility/Utility.h"
#include "../Common/Geometry/Sphere.h"
#include "../Common/Geometry/Capsule.h"
#include "../Common/Geometry/Cube.h"
#include "../Common/Geometry/Line.h"
#include "../Common/Geometry/Model.h"
#include "StageObject.h"

StageObject::StageObject(const StageImportData& _data,const int _modelId, const Collider::TAG _tag)
{
	trans_.modelId = _modelId;
	trans_.pos = _data.position;
	trans_.scl = _data.scale;
	trans_.quaRot = _data.quaternion;

	data_ = _data;
	data_.localPos1.x = data_.localPos1.x * data_.scale.x;
	data_.localPos1.y = data_.localPos1.y * data_.scale.y;
	data_.localPos1.z = data_.localPos1.z * data_.scale.z;
	data_.localPos2.x = data_.localPos2.x * data_.scale.x;
	data_.localPos2.y = data_.localPos2.y * data_.scale.y;
	data_.localPos2.z = data_.localPos2.z * data_.scale.z;

	createGeo_["sphere"] = [this](void) { return MakeSphere(); };
	createGeo_["capsule"] = [this](void) { return MakeCapsule(); };
	createGeo_["cube"] = [this](void) { return MakeCube(); };
	createGeo_["line"] = [this](void) { return MakeLine(); };
	createGeo_["model"] = [this](void) { return MakeModel(); };
	
	//コライダ生成
	std::unique_ptr<Geometry> geo = createGeo_[data_.geometry]();
	MakeCollider(_tag, std::move(geo), { Collider::TAG::GROUND,Collider::TAG::NORMAL_OBJECT });

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
	//初期更新
	Update();
}

void StageObject::Update(void)
{
	trans_.Update();
}

void StageObject::Draw(void)
{
	collider_[0]->GetGeometry().Draw(color_);

	MV1DrawModel(trans_.modelId);

	color_ = 0;
}

void StageObject::OnHit(const std::weak_ptr<Collider> _hitCol)
{
	color_ = 0xff0000;
}

std::unique_ptr<Geometry> StageObject::MakeSphere(void)
{
	return std::make_unique<Sphere>(trans_.pos, trans_.pos, data_.radius);
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
