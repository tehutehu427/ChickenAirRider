#include "../Utility/Utility.h"
#include "../Common/Geometry/Geometry.h"
#include "StageObject.h"

StageObject::StageObject(const StageImportData& _data,const int _modelId)
{
	//ƒ‚ƒfƒ‹‚Ìİ’è
	trans_.modelId = _modelId;
	trans_.pos = _data.position;
	trans_.scl = _data.scale;
	trans_.quaRot = _data.quaternion;
}

StageObject::~StageObject(void)
{
}

void StageObject::Load(void)
{
}

void StageObject::Init(void)
{
	//‰ŠúXV
	Update();
}

void StageObject::Update(void)
{
	trans_.Update();
}

void StageObject::Draw(void)
{
	MV1DrawModel(trans_.modelId);
}

void StageObject::OnHit(const Collider* _hitCol)
{
}

void StageObject::CreateCollider(const Collider::TAG _tag, std::unique_ptr<Geometry> _geo)
{
	//“–‚½‚è”»’è‚Ìì¬
	MakeCollider(_tag, std::move(_geo), { Collider::TAG::GROUND,Collider::TAG::NORMAL_OBJECT, _tag });
}