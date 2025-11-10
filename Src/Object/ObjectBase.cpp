#include"../pch.h"
#include "../Manager/System/ResourceManager.h"
#include "../Manager/System/SceneManager.h"
#include "../Manager/Game/CollisionManager.h"
#include "Common/EffectController.h"
#include "ObjectBase.h"

ObjectBase::ObjectBase(void)
	: resMng_(ResourceManager::GetInstance()),
	scnMng_(SceneManager::GetInstance())
{
	//エフェクト
	effect_ = std::make_unique<EffectController>();
}

ObjectBase::~ObjectBase(void)
{
	for (auto& collider : collider_)
	{
		//所持している全コライダの削除
		collider->Kill();
	}

	collider_.clear();
}

void ObjectBase::Sweep(void)
{
	//削除された判定を配列から破棄
	std::erase_if(collider_, [](std::weak_ptr<Collider> _collider) {return _collider.lock()->IsDead(); });
}

void ObjectBase::ChangeModelColor(const COLOR_F _colorScale)
{
	if (MV1SetDifColorScale(trans_.modelId, _colorScale))
	{
#ifdef _DEBUG

		OutputDebugString(L"ChangeModelColorの失敗");

#endif // _DEBUG
	}
}

void ObjectBase::MakeCollider(const Collider::TAG _tag, std::unique_ptr<Geometry> _geometry, const std::set<Collider::TAG> _notHitTags)
{
	//情報を使ってコライダの作成
	std::shared_ptr<Collider> collider = std::make_shared<Collider>(*this, _tag, std::move(_geometry), _notHitTags);

	//コライダを管理マネージャーに追加
	CollisionManager::GetInstance().AddCollider(collider);

	//配列に格納
	collider_.push_back(collider);
}

void ObjectBase::DeleteAllCollider(void)
{
	for (auto& collider : collider_)
	{
		if (collider == nullptr)continue;
		collider->Kill();
	}
	collider_.clear();
}
