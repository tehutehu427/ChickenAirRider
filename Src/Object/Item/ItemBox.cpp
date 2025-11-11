#include "../pch.h"
#include "../Manager/System/ResourceManager.h"
#include "../Object/Common/Geometry/Cube.h"
#include "ItemBox.h"

ItemBox::ItemBox(VECTOR _pos)
{
	trans_.pos = _pos;
	isDead_ = false;
}

ItemBox::~ItemBox(void)
{
}

void ItemBox::Load(void)
{
	//モデル設定
	trans_.modelId = ResourceManager::GetInstance().LoadModelDuplicate(ResourceManager::SRC::ITEM_BOX);
}

void ItemBox::Init(void)
{
	//当たり判定
	std::unique_ptr<Geometry> geo = std::make_unique<Cube>(trans_.pos, trans_.quaRot, BOX_HALF);
	MakeCollider(Collider::TAG::NORMAL_OBJECT, std::move(geo));
}

void ItemBox::Update(void)
{
	//モデル更新
	trans_.Update();
}

void ItemBox::Draw(void)
{
	//モデル描画
	MV1DrawModel(trans_.modelId);
}

void ItemBox::OnHit(std::weak_ptr<Collider> _hitCol)
{
	if (_hitCol.lock()->GetTag() == Collider::TAG::PLAYER1)
	{
		//コライダ削除
		collider_[0]->Kill();

		//死亡判定
		isDead_ = true;
	}
}
