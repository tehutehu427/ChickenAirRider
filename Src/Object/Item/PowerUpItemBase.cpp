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
	//パラメーター
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
	//コライダ生成(接地用)
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
	//状態ごとの更新
	(this->*update_[static_cast<int>(state_)])();
}

void PowerUpItemBase::Draw(void)
{
	ItemBase::Draw();
}

void PowerUpItemBase::OnHit(const std::weak_ptr<Collider> _hitCol)
{
	ItemBase::OnHit(_hitCol);
}

void PowerUpItemBase::UpdateAlive(void)
{
	//移動後座標更新
	trans_.pos = movedPos_;

	//カウンタ
	const auto& delta = SceneManager::GetInstance().GetDeltaTime();

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

void PowerUpItemBase::UpdateGot(void)
{
	//移動後座標更新
	trans_.pos = movedPos_;

	//カウンタ
	const auto& delta = SceneManager::GetInstance().GetDeltaTime();

	//表示カウンタ
	displayCnt_ += delta;

	//表示時間
	if (displayCnt_ > GOT_DISPLAY_TIME)
	{
		//死亡
		state_ = STATE::DEAD;
	}
}

void PowerUpItemBase::UpdateDead(void)
{
	//何もしない
}