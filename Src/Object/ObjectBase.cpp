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
	for (auto& colParam : colParam_)
	{
		//所持している全コライダの削除
		colParam.collider_->Kill();
	}

	colParam_.clear();
}

void ObjectBase::Sweep(void)
{
	//削除された判定を配列から破棄
	std::erase_if(colParam_, [](ColParam& _colParam) {return _colParam.collider_->IsDead(); });
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

void ObjectBase::MakeCollider(const std::set<Collider::TAG> _tag, std::unique_ptr<Geometry> _geometry, const std::set<Collider::TAG> _notHitTags)
{
	//当たり判定情報
	ColParam colParam;

	//形状情報の挿入
	colParam.geometry_ = std::move(_geometry);

	//情報を使ってコライダの作成
	colParam.collider_ = std::make_shared<Collider>(*this, _tag, *colParam.geometry_,_notHitTags);

	//コライダを管理マネージャーに追加
	CollisionManager::GetInstance().AddCollider(colParam.collider_);

	//配列にセット
	colParam_.push_back(std::move(colParam));
}

void ObjectBase::DeleteAllCollider(void)
{
	for (auto& col : colParam_)
	{
		if (col.collider_ == nullptr)continue;
		col.collider_->Kill();
	}
	colParam_.clear();
}
