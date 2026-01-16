#include "../pch.h"
#include "../Manager/System/SceneManager.h"
#include "../Manager/Game/GravityManager.h"
#include "../Utility/Utility.h"
#include "../Object/Common/Geometry/Sphere.h"
#include "BattleItemBase.h"

BattleItemBase::BattleItemBase(const VECTOR& _pos, const VECTOR& _vec)
	: ItemBase(_pos,_vec)
{
	imgId_ = -1;
	modelScl_ = Utility::VECTOR_ONE;
}

BattleItemBase::~BattleItemBase(void)
{
}

void BattleItemBase::Load(void)
{
}

void BattleItemBase::Init(void)
{
	//コライダ生成
	std::unique_ptr<Geometry> geo = std::make_unique<Sphere>(trans_.pos, trans_.pos, OBJECT_HIT_RADIUS);
	MakeCollider(Collider::TAG::BATTLE_ITEM, std::move(geo),
		{ Collider::TAG::POWER_UP
		,Collider::TAG::PLAYER1
		,Collider::TAG::PLAYER2
		,Collider::TAG::PLAYER3
		,Collider::TAG::PLAYER4
		,Collider::TAG::SPIN
		});

	ItemBase::Init();
}

void BattleItemBase::Update(void)
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
			std::unique_ptr<Geometry> geo = std::make_unique<Sphere>(trans_.pos, movedPos_, PLAYER_HIT_RADIUS);
			MakeCollider(Collider::TAG::BATTLE_ITEM, std::move(geo), { Collider::TAG::BATTLE_ITEM,Collider::TAG::NORMAL_OBJECT,Collider::TAG::GROUND });
		
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
		if (displayCnt_ > ACTIVE_TIME)
		{
			//死亡
			state_ = STATE::DEAD;
		}

		//モデル更新
		trans_.pos = VAdd(hiter_.lock()->GetParent().GetTrans().pos, VGet(0.0f, LOCAL_POS_Y, 0.0f));
		trans_.quaRot = hiter_.lock()->GetParent().GetTrans().quaRot;
		trans_.scl = modelScl_;//VScale(modelScl_, hiter_.lock()->GetParent().GetTrans().scl.x);
		trans_.Update();
	}
}

void BattleItemBase::Draw(void)
{
	//死亡ならスキップ
	if (state_ == STATE::DEAD)return;

	//生存中なら
	else if (state_ == STATE::ALIVE)
	{
		//大きく描画
		DrawBillboard3D(trans_.pos, 0.5f, 0.5f, ALIVE_IMG_SIZE, 0.0f, imgId_, true);
	}
	else
	{
		//取得者がいないとスキップ
		if (hiter_.lock() == nullptr)return;

		//取得者の頭上にモデル描画
		MV1DrawModel(trans_.modelId);
	}
}

void BattleItemBase::OnHit(std::weak_ptr<Collider> _hitCol)
{
	ItemBase::OnHit(_hitCol);
}
