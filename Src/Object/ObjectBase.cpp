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

	//半径
	broudRadius_ = 0.0f;
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
	std::erase_if(collider_, [](std::unique_ptr<Collider>& _collider) {return _collider->IsDead(); });
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
	std::unique_ptr<Collider> collider = std::make_unique<Collider>(*this, _tag, std::move(_geometry), _notHitTags);

	//コライダを管理マネージャーに追加
	CollisionManager::GetInstance().AddCollider(collider.get());

	//配列に格納
	collider_.push_back(std::move(collider));
}

void ObjectBase::DeleteColliderAtTag(Collider::TAG _tag)
{
	for (auto& collider : collider_)
	{
		//そのコライダが存在しない　又は　指定タグと違う
		if (collider == nullptr || collider->GetTag() != _tag)continue;
		collider->Kill();
	}

	//削除
	std::erase_if(collider_, [](std::unique_ptr<Collider>& _collider) {return _collider->IsDead(); });
}

void ObjectBase::DeleteAllCollider(void)
{
	for (auto& collider : collider_)
	{
		//そのコライダが存在しない
		if (collider == nullptr)continue;
		collider->Kill();
	}
	collider_.clear();
}
