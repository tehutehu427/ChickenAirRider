#include "../pch.h"
#include "../Utility/Utility.h"
#include "../Manager/System/ResourceManager.h"
#include "SkyDome.h"

SkyDome::SkyDome(const VECTOR& _follow)
	: follow_(_follow)
{
	modelId_ = -1;
	rot_ = Utility::VECTOR_ZERO;
	scale_ = Utility::VECTOR_ZERO;
}

SkyDome::~SkyDome(void)
{
}

void SkyDome::Load(void)
{
	//インスタンス
	auto& res = ResourceManager::GetInstance();

	//モデル情報
	modelId_ = res.LoadModelDuplicate(ResourceManager::SRC::SKY_DOME);
}

void SkyDome::Init(void)
{
	//大きさ
	scale_ = Utility::VECTOR_ONE;

	//モデル更新
	Update();
}

void SkyDome::Update(void)
{
	//モデル制御
	MV1SetPosition(modelId_, VAdd(follow_, LOCAL_POS));
	MV1SetScale(modelId_, scale_);
	MV1SetRotationXYZ(modelId_, rot_);
}

void SkyDome::Draw(void)
{
	//描画
	MV1DrawModel(modelId_);
}
