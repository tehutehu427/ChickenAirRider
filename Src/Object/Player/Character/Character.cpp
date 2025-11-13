#include"../pch.h"
#include"../Utility/Utility.h"
#include"../../Manager/System/SceneManager.h"
#include"../../Manager/Game/AnimationManager.h"
#include"../../Manager/System/ResourceManager.h"
#include "Character.h"

Character::Character(void)
{
	trans_ = Transform();
}

Character::~Character(void)
{
}

void Character::Load(void)
{
	//キャラクターのステータスを外部から持ってくる
	unitParam_.fixedMaxSpeed_ = PARAM_NORMAL;
	unitParam_.fixedAcceleration_ = PARAM_NORMAL;
	unitParam_.fixedTurning_ = PARAM_NORMAL;
	unitParam_.fixedCharge_ = PARAM_NORMAL;
	unitParam_.fixedFlight_ = PARAM_NORMAL;
	unitParam_.fixedWeight_ = PARAM_NORMAL;
	unitParam_.fixedAttack_ = PARAM_NORMAL;
	unitParam_.fixedDefence_ = PARAM_NORMAL;
	unitParam_.fixedMaxHealth_ = PARAM_NORMAL;

	unitParam_.affectMaxSpeed_ = 0.5f;
	unitParam_.affectAcceleration_ = 0.5f;
	unitParam_.affectTurning_ = 0.5f;
	unitParam_.affectCharge_ = 0.5f;
	unitParam_.affectFlight_ = 0.5f;
	unitParam_.affectWeight_ = 0.5f;
	unitParam_.affectAttack_ = 0.5f;
	unitParam_.affectDefence_ = 0.5f;
	unitParam_.affectMaxHealth_ = 0.5f;

	unitParam_.chargeBraking_ = 0.2f;
	unitParam_.chargeCapacity_ = 100.0f;

	//モデル
	trans_.quaRotLocal = Quaternion::AngleAxis(Utility::Deg2RadF(180.0f), Utility::AXIS_Y);
	trans_.SetModel(ResourceManager::GetInstance().Load(ResourceManager::SRC::CHICKEN).handleId_);
	
	//アニメーション
	anim_ = std::make_unique<AnimationController>(trans_.modelId);

	//アニメーション番号
	const auto& animNums = AnimationManager::GetInstance().GetAnimationData("chicken");
	const auto& delta = SceneManager::GetInstance().GetDeltaTime();

	//アニメーション割り当て
	for (const auto& animNum : animNums)
	{
		anim_->Add(animNum.first, animNum.second, 60.0f);
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
	//DrawSphere3D(trans_.pos, 10.0f, 20, 0xffff00, 0xffff00, true);
}
