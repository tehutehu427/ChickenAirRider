#include "../pch.h"
#include"../Manager/System/SceneManager.h"
#include"../Manager/System/ResourceManager.h"
#include "CannonShot.h"
#include "Cannon.h"

Cannon::Cannon(const VECTOR& _pos, const VECTOR& _vec)
	: BattleItemBase(_pos,_vec)
{
	createCnt_ = 0.0f;
}

Cannon::~Cannon(void)
{
}

void Cannon::Load(void)
{
	imgId_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::CANNON_IMAGE).handleId_;
	trans_.modelId = ResourceManager::GetInstance().LoadModelDuplicate(ResourceManager::SRC::CANNON_MODEL);
}

void Cannon::Init(void)
{
	modelScl_ = { SCALE ,SCALE ,SCALE };
	BattleItemBase::Init();
}

void Cannon::Update(void)
{
	//生成カウンタ
	createCnt_ += SceneManager::GetInstance().GetDeltaTime();

	for (auto& shot : shots_)
	{
		shot->Update();
	}

	//取得中なら
	if (state_ == STATE::GOT)
	{
		if (createCnt_ > CREATE_TIME)
		{
			CreateShot();
			createCnt_ = 0.0f;
		}
	}

	//共通更新
	BattleItemBase::Update();
}

void Cannon::Draw(void)
{
	for (auto& shot : shots_)
	{
		shot->Draw();
	}

	BattleItemBase::Draw();
}

void Cannon::CreateShot(void)
{
	for (auto& shot : shots_)
	{
		//上書き
		if (!shot->IsAlive())
		{
			shot.reset();
			shot = std::make_unique<CannonShot>(trans_.pos, trans_.quaRot, trans_.scl, hiter_);
			shot->Load();
			shot->Init();

			return;
		}
	}

	//弾を生成する
	std::unique_ptr<CannonShot>shot = std::make_unique<CannonShot>(trans_.pos, trans_.quaRot, trans_.scl, hiter_);
	shot->Load();
	shot->Init();

	//配列追加
	shots_.emplace_back(std::move(shot));
}
