#include "../pch.h"
#include "../Utility/Utility.h"
#include "../Object/Common/Geometry/Sphere.h"
#include "../Object/Common/Geometry/Model.h"
#include "ItemBase.h"

ItemBase::ItemBase(const VECTOR& _pos, const VECTOR& _vec)
{
	trans_.pos = Utility::VECTOR_ZERO;
	vec_ = _vec;
	displayCnt_ = 0.0f;
	state_ = STATE::ALIVE;
	createColCnt_ = 0.0f;
	movePow_ = Utility::VECTOR_ZERO;
	gravPow_ = Utility::VECTOR_ZERO;
	movedPos_ = _pos;
	isCreateCol_ = false;
}

ItemBase::~ItemBase(void)
{
	//所持情報のみ破棄(本体は消さない)
	hiter_ = nullptr;
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

	//初期化
	broudRadius_ = BROUD_RADIUS;
	isCreateCol_ = false;

	Update();
}

void ItemBase::Update(void)
{
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
		if (hiter_ == nullptr)return;

		//取得者の座標
		VECTOR hiterPos = hiter_->GetOwner().GetTrans().pos;
		Sphere& sphere = dynamic_cast<Sphere&>(hiter_->GetGeometry());
		
		//小さめに描画
		DrawBillboard3D(VGet(hiterPos.x, hiterPos.y + sphere.GetRadius() + LOCAL_HITER_POS_Y, hiterPos.z), 0.5f, 0.5f, GOT_IMG_SIZE, 0.0f, trans_.modelId, true);
	}
}

void ItemBase::OnHit(const Collider& _hitCol)
{
	if (_hitCol.GetTag() == Collider::TAG::PLAYER1
		|| _hitCol.GetTag() == Collider::TAG::PLAYER2
		|| _hitCol.GetTag() == Collider::TAG::PLAYER3
		|| _hitCol.GetTag() == Collider::TAG::PLAYER4
		)
	{
		//コライダ削除
		for (auto& col : collider_)
		{
			col->Kill();
		}

		//死亡判定
		state_ = STATE::GOT;

		//取得者
		hiter_ = &_hitCol;
	}
	else if (_hitCol.GetTag() == Collider::TAG::NORMAL_OBJECT)
	{
		//コライダ
		auto& mainCol = collider_[static_cast<int>(COL_VALUE::OBJECT)];

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

		//移動しなくなる
		movePow_ = Utility::VECTOR_ZERO;
	}
	else if (_hitCol.GetTag() == Collider::TAG::GROUND)
	{
		//コライダ
		auto& mainCol = collider_[static_cast<int>(COL_VALUE::OBJECT)];

		//相手モデル
		Model& model = dynamic_cast<Model&>(_hitCol.GetGeometry());
		const int hitNum = model.GetHitInfo().HitNum;

		//自身の球
		Sphere& mainSphere = dynamic_cast<Sphere&>(mainCol->GetGeometry());
		float radius = mainSphere.GetRadius();

		//移動後座標
		VECTOR pos = movedPos_;
		VECTOR totalNormal = VGet(0, 0, 0);
		float maxDepth = 0.0f;

		for (int i = 0; i < hitNum; i++)
		{
			//当たった座標
			VECTOR hitPos = mainSphere.GetHitInfo().Dim[i].HitPosition;

			//法線
			VECTOR normal = VNorm(mainSphere.GetHitInfo().Dim[i].Normal);

			//深度
			float depth = radius - VDot(normal, VSub(pos, hitPos));

			//めり込んでるなら
			if (depth > 0.0f)
			{
				//保存
				totalNormal = VAdd(totalNormal, normal);
				maxDepth = std::max(maxDepth, depth);
			}
		}

		// 押し戻し
		if (maxDepth > 0.0f)
		{
			VECTOR N = VNorm(totalNormal);
			pos = VAdd(pos, VScale(N, maxDepth));
		}

		//適用する
		movedPos_ = pos;

		//移動しなくなる
		movePow_ = Utility::VECTOR_ZERO;
	}
}
