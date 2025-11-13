#include "../pch.h"
#include "../Manager/System/ResourceManager.h"
#include "../Manager/Game/ItemManager.h"
#include "../Object/Common/Geometry/Sphere.h"
#include "ItemBase.h"

ItemBase::ItemBase(const VECTOR _pos, const Parameter& _param, const int _imageId)
{
	trans_.pos = _pos;
	param_ = _param;
	trans_.modelId = _imageId;
	isDead_ = false;
}

ItemBase::~ItemBase(void)
{
}

void ItemBase::Load(void)
{
}

void ItemBase::Init(void)
{
	//コライダ生成
	std::unique_ptr<Geometry> geo = std::make_unique<Sphere>(trans_.pos, trans_.pos, RADIUS);
	MakeCollider(Collider::TAG::POWER_UP, std::move(geo));
}

void ItemBase::Update(void)
{
}

void ItemBase::Draw(void)
{
	//画像描画
	DrawBillboard3D({ trans_.pos.x,trans_.pos.y,trans_.pos.z }, 0.5f, 0.5f, IMG_SIZE, 0.0f, trans_.modelId, true);
}

void ItemBase::OnHit(std::weak_ptr<Collider> _hitCol)
{
	if (_hitCol.lock()->GetTag() == Collider::TAG::PLAYER1)
	{
		//コライダ削除
		collider_[0]->Kill();

		//死亡判定
		isDead_ = true;
	}
}
