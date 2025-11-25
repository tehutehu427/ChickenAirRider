#include "../pch.h"
#include "../Utility/Utility.h"
#include "../Manager/System/ResourceManager.h"
#include "../Manager/System/SceneManager.h"
#include "../Manager/Game/GravityManager.h"
#include "../Manager/Game/ItemManager.h"
#include "../Object/Common/Geometry/Sphere.h"
#include "ItemBase.h"

ItemBase::ItemBase(const VECTOR& _pos, const VECTOR& _vec, const Parameter& _param, const int _imageId)
{
	trans_.pos = Utility::VECTOR_ZERO;
	vec_ = _vec;
	param_ = _param;
	trans_.modelId = _imageId;
	displayCnt_ = 0.0f;
	state_ = STATE::ALIVE;
	moveCnt_ = 0.0f;
	movePow_ = Utility::VECTOR_ZERO;
	gravPow_ = Utility::VECTOR_ZERO;
	movedPos_ = _pos;
}

ItemBase::~ItemBase(void)
{
	hiter_.reset();
}

void ItemBase::Load(void)
{
}

void ItemBase::Init(void)
{
	//移動量
	movePow_.x = vec_.x * static_cast<float>(Utility::GetRandomValue(MOVE_POW_MIN, MOVE_POW_MAX));
	gravPow_.y = MOVE_POW_Y;
	movePow_.z = vec_.z * static_cast<float>(Utility::GetRandomValue(MOVE_POW_MIN, MOVE_POW_MAX));
}

void ItemBase::Update(void)
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
			MakeCollider(Collider::TAG::POWER_UP, std::move(geo));
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
		if (displayCnt_ > GOT_DISPLAY_TIME)
		{
			//死亡
			state_ = STATE::DEAD;
		}
	}
}

void ItemBase::Draw(void)
{
	//死亡ならスキップ
	if (state_ == STATE::DEAD)return;

	//生存中なら
	else if (state_ == STATE::ALIVE)
	{
		//大きく描画
		DrawBillboard3D(trans_.pos, 0.5f, 0.5f, ALIVE_IMG_SIZE, 0.0f, trans_.modelId, true);
	}
	else
	{
		//取得者がいないとスキップ
		if (hiter_.lock() == nullptr)return;

		//取得者の座標
		VECTOR hiterPos = hiter_.lock()->GetParent().GetTrans().pos;

		//小さめに描画
		DrawBillboard3D(VGet(hiterPos.x, hiterPos.y + LOCAL_HITER_POS_Y, hiterPos.z), 0.5f, 0.5f, GOT_IMG_SIZE, 0.0f, trans_.modelId, true);
	}
}

void ItemBase::OnHit(std::weak_ptr<Collider> _hitCol)
{
	if (_hitCol.lock()->GetTag() == Collider::TAG::PLAYER1)
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
