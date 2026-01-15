#include "../pch.h"
#include "../Utility/Utility.h"
#include "../Manager/System/ResourceManager.h"
#include "SkyDome.h"

SkyDome::SkyDome(void)
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
	scale_ = VGet(SCALE, SCALE, SCALE);

	//モデル更新
	Update();
}

void SkyDome::Update(void)
{
	//モデル制御
	MV1SetScale(modelId_, scale_);
	MV1SetRotationXYZ(modelId_, rot_);
}

void SkyDome::Draw(const VECTOR& _pos)
{
	//位置
	MV1SetPosition(modelId_, _pos);

	//描画
	MV1DrawModel(modelId_);
}
