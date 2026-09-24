#include"../pch.h"
#include"../Application.h"
#include"../Utility/Utility.h"
#include"../../Manager/System/SceneManager.h"
#include"../../Manager/Game/AnimationManager.h"
#include"../../Manager/System/ResourceManager.h"
#include "CharacterImportData.h"
#include "../AnimationImportData.h"
#include "Character.h"

Character::Character(void)
{
	trans_ = Transform();
}

Character::~Character(void)
{

}

void Character::Load(const CharacterImportData& _data, const int _modelId)
{
	//パラメータ情報
	unitParam_ = _data.param;

	//モデルID
	trans_.modelId = _modelId;

	//アニメーション
	anim_ = std::make_unique<AnimationController>(trans_.modelId);

	//アニメーション番号
	const auto& animNums = AnimationManager::GetInstance().GetAnimationData(_data.name);

	//アニメーション割り当て
	for (const auto& animNum : animNums)
	{
		anim_->Add(animNum.first, animNum.second.number, animNum.second.speed);
	}
}

void Character::Init(void)
{
}

void Character::Update(void)
{
	//モデル用
	trans_.Update();

	//アニメーション
	anim_->Update();
}

void Character::Draw(void)
{
	//モデル描画
	MV1DrawModel(trans_.modelId);
}
