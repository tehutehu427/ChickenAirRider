#include "../pch.h"
#include"../Utility/Utility.h"
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
	for (auto& shot : shots_)
	{
		shot->Update();
	}

	//取得中なら
	if (state_ == STATE::GOT)
	{
		//生成カウンタ
		createCnt_ += SceneManager::GetInstance().GetDeltaTime();

		if (createCnt_ > CREATE_TIME)
		{
			CreateShot();
			createCnt_ = 0.0f;
		}
	}

	//共通更新
	BattleItemBase::Update();

	trans_.quaRot = trans_.quaRot.Mult(Quaternion::Euler(VGet(Utility::Deg2RadF(ANGLE_X),0.0f, 0.0f)));
	trans_.Update();
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
	//速度
	VECTOR hiterPos = hiter_->GetOwner()->GetTrans().pos;
	float speed = Utility::Distance(VGet(hiterPos.x, hiterPos.y + LOCAL_POS_Y, hiterPos.z), trans_.pos);
	VECTOR localPos = VSub(VGet(hiterPos.x, hiterPos.y + LOCAL_POS_Y, hiterPos.z), trans_.pos);
	VECTOR createPos = trans_.pos; 

	for (auto& shot : shots_)
	{
		//上書き
		if (!shot->IsAlive())
		{
			shot.reset();
			shot = std::make_unique<CannonShot>(createPos, trans_.quaRot, trans_.scl, *hiter_, speed);
			shot->Load();
			shot->Init();

			return;
		}
	}

	//弾を生成する
	std::unique_ptr<CannonShot>shot = std::make_unique<CannonShot>(createPos, trans_.quaRot, trans_.scl, *hiter_, speed);
	shot->Load();
	shot->Init();

	//配列追加
	shots_.emplace_back(std::move(shot));
}
