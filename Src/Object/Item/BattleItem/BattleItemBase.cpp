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
		if (moveCnt_ > NO_HIT_TIME && collider_.empty())
		{
			//コライダ生成
			std::unique_ptr<Geometry> geo = std::make_unique<Sphere>(trans_.pos, trans_.pos, RADIUS);
			MakeCollider(Collider::TAG::BATTLE_ITEM, std::move(geo));
		}

		//重力
		GravityManager::GetInstance().CalcGravity(Utility::DIR_D, gravPow_);

		//移動時間を超えたならスキップ
		if (moveCnt_ > MOVE_TIME)return;

		//カウンタ
		moveCnt_ += delta;

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
		trans_.pos = VAdd(hiter_.lock()->GetParent().GetTrans().pos, VGet(0.0f, 30.0f, 0.0f));
		trans_.quaRot = hiter_.lock()->GetParent().GetTrans().quaRot;
		trans_.scl = VScale(modelScl_, hiter_.lock()->GetParent().GetTrans().scl.x);
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
	if (_hitCol.lock()->GetTag() == Collider::TAG::PLAYER1
		|| _hitCol.lock()->GetTag() == Collider::TAG::PLAYER2
		|| _hitCol.lock()->GetTag() == Collider::TAG::PLAYER3
		|| _hitCol.lock()->GetTag() == Collider::TAG::PLAYER4
		)
	{
		//コライダ削除
		collider_[0]->Kill();

		//死亡判定
		state_ = STATE::GOT;

		//取得者
		hiter_ = _hitCol;
	}
	else if (_hitCol.lock()->GetTag() == Collider::TAG::NORMAL_OBJECT)
	{
		//コライダ
		auto& mainCol = collider_[static_cast<int>(COL_VALUE::MAIN)];

		//位置の補正
		const auto& hit = mainCol->GetGeometry().GetHitResult();

		//移動前から移動後までのベクトル
		VECTOR v = VSub(movedPos_, trans_.pos);

		// 衝突時位置まで移動
		movedPos_ = VAdd(trans_.pos, VScale(v, hit.t));

		// 少し押し戻す（めり込み回避）
		movedPos_ = VAdd(trans_.pos, VScale(hit.normal, 0.007f));

		// 残り移動をスライド方向へ
		float remain = 1.0f - hit.t;
		VECTOR slide = VSub(v, VScale(hit.normal, VDot(v, hit.normal)));
		movedPos_ = VAdd(movedPos_, VScale(slide, remain));
	}
}
