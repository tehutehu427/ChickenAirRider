#include "../Common/Geometry/Cube.h"
#include "Stage.h"

Stage::Stage(void)
{
}

Stage::~Stage(void)
{
}

void Stage::Load(void)
{
}

void Stage::Init(void)
{
	//ÉRÉâÉCÉ_ê∂ê¨
	std::unique_ptr<Cube> geo = std::make_unique<Cube>(trans_.pos, trans_.quaRot, STAGE_SIZE);
	MakeCollider({ Collider::TAG::STAGE }, std::move(geo));
}

void Stage::Update(void)
{
}

void Stage::Draw(void)
{
	colParam_[0].geometry_->Draw();
}

void Stage::OnHit(const std::weak_ptr<Collider> _hitCol)
{
}
