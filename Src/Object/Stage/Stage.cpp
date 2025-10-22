#include "../Utility/Utility.h"
#include "../Manager/System/ResourceManager.h"
#include "../Common/Geometry/Cube.h"
#include "Stage.h"

Stage::Stage(void)
{
	color_ = 0;
}

Stage::~Stage(void)
{
}

void Stage::Load(void)
{
	auto& res = ResourceManager::GetInstance();

	trans_.modelId = res.LoadModelDuplicate(ResourceManager::SRC::GLASS);
	trans_.localPos.y = -MODEL_SIZE_Y / 2.0f;
}

void Stage::Init(void)
{
	trans_.pos.y = -100.0f;

	//ÉRÉâÉCÉ_ê∂ê¨
	std::unique_ptr<Geometry> geo = std::make_unique<Cube>(trans_.pos, trans_.quaRot, STAGE_SIZE);
	MakeCollider({ Collider::TAG::STAGE }, std::move(geo));
}

void Stage::Update(void)
{
	trans_.Update();
}

void Stage::Draw(void)
{
	colParam_[0].geometry_->Draw(color_);

	DrawSphere3D(VGet(0.0f, 0.0f, 1000.0f), 100.0f, 20, color_, color_, true);

	MV1DrawModel(trans_.modelId);

	color_ = 0;
}

void Stage::OnHit(const std::weak_ptr<Collider> _hitCol)
{
	color_ = 0xff0000;
}
