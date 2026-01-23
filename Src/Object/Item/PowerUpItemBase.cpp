#include "../pch.h"
#include "../Manager/System/SceneManager.h"
#include "../Manager/Game/GravityManager.h"
#include "../Manager/Game/ItemManager.h"
#include "../Utility/Utility.h"
#include "../Object/Common/Geometry/Sphere.h"
#include "PowerUpItemBase.h"

PowerUpItemBase::PowerUpItemBase(const VECTOR& _pos, const VECTOR& _vec, const int _imageId, const Parameter& _param)
	: ItemBase(_pos,_vec)
{
	param_ = _param;
	trans_.modelId = _imageId;
}

PowerUpItemBase::~PowerUpItemBase(void)
{
}

void PowerUpItemBase::Load(void)
{
}

void PowerUpItemBase::Init(void)
{
	//コライダ生成
	std::unique_ptr<Geometry> geo = std::make_unique<Sphere>(trans_.pos, trans_.pos, OBJECT_HIT_RADIUS);
	MakeCollider(Collider::TAG::POWER_UP, std::move(geo),
		{ Collider::TAG::POWER_UP
		,Collider::TAG::PLAYER1
		,Collider::TAG::PLAYER2
		,Collider::TAG::PLAYER3
		,Collider::TAG::PLAYER4
		,Collider::TAG::SPIN
		});

	ItemBase::Init();
}

void PowerUpItemBase::Update(void)
{
	//死亡ならスキップ
	if (state_ == STATE::DEAD)return;

	//カウンタ
	const auto& delta = SceneManager::GetInstance().GetDeltaTime();
	trans_.pos = movedPos_;

	//生存なら移動
	if (state_ == STATE::ALIVE)
	{
		//生成から少し置いてコライダ生成
		if (createColCnt_ > CREATE_COL_TIME && !isCreateCol_)
		{
			//コライダ生成
			std::unique_ptr<Geometry>geo = std::make_unique<Sphere>(trans_.pos, trans_.pos, PLAYER_HIT_RADIUS);
			MakeCollider(Collider::TAG::POWER_UP, std::move(geo), { Collider::TAG::POWER_UP,Collider::TAG::NORMAL_OBJECT,Collider::TAG::GROUND });

			isCreateCol_ = true;
		}

		//重力
		GravityManager::GetInstance().CalcGravity(Utility::DIR_D, gravPow_);

		//カウンタ
		createColCnt_ += delta;

		//移動
		movedPos_ = VAdd(movedPos_, movePow_);
		movedPos_ = VAdd(movedPos_, gravPow_);
	}

	//取得なら表示
	else if (state_ == STATE::GOT)
	{
		displayCnt_ += delta;

		//表示時間
		if (displayCnt_ > GOT_DISPLAY_TIME)
		{
			//死亡
			state_ = STATE::DEAD;
		}
	}
}

void PowerUpItemBase::Draw(void)
{
	ItemBase::Draw();
}

void PowerUpItemBase::OnHit(const Collider& _hitCol)
{
	ItemBase::OnHit(_hitCol);
}
