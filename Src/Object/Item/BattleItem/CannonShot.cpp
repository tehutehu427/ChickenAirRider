#include "../pch.h"
#include "../Manager/System/SceneManager.h"
#include "../Manager/System/ResourceManager.h"
#include"../Manager/Game/GravityManager.h"
#include "../Utility/Utility.h"
#include "../../Common/Geometry/Sphere.h"
#include "../../Player/Player.h"
#include "CannonShot.h"

CannonShot::CannonShot(const VECTOR& _pos, const Quaternion& _rot, const VECTOR& _scl, const std::weak_ptr<Collider> _holder, const float _speed)
{
	movedPos_ = _pos;
	trans_.pos = _pos;
	trans_.quaRot = _rot;
	trans_.scl = VScale(_scl,0.3f);
	holder_ = _holder;
	speed_ = _speed + SPEED;
	gravPow_ = Utility::VECTOR_ZERO;
	movePow_ = Utility::VECTOR_ZERO;
	aliveCnt_ = 0.0f;
	state_ = STATE::ALIVE;
	attack_ = 0.0f;

	update_.emplace(STATE::ALIVE, [this](void) {UpdateAlive(); });
	update_.emplace(STATE::BLAST, [this](void) {UpdateBlast(); });
	update_.emplace(STATE::DEAD, [this](void) {UpdateDead(); });

	draw_.emplace(STATE::ALIVE, [this](void) {DrawAlive(); });
	draw_.emplace(STATE::BLAST, [this](void) {DrawBlast(); });
	draw_.emplace(STATE::DEAD, [this](void) {DrawDead(); });
}

CannonShot::~CannonShot(void)
{
}

void CannonShot::Load(void)
{
	//モデル
	trans_.modelId = ResourceManager::GetInstance().LoadModelDuplicate(ResourceManager::SRC::CANNON_SHOT_MODEL);
}

void CannonShot::Init(void)
{
	//初期化
	broudRadius_ = BROUD_RADIUS;
	aliveCnt_ = 0.0f;
	blastCnt_ = 0.0f;
	state_ = STATE::ALIVE;
	gravPow_ = Utility::VECTOR_ZERO;
	movePow_ = Utility::VECTOR_ZERO;

	//所持者
	const auto& holder = holder_.lock();

	//攻撃力
	attack_ = dynamic_cast<const Player&>(holder->GetOwner()).GetAttack() * ATTACK_MULTI;

	//コライダ
	std::unique_ptr<Geometry> geo = std::make_unique<Sphere>(trans_.pos, movedPos_, SHOT_RADIUS);
	MakeCollider(Collider::TAG::CANNON_SHOT, std::move(geo), { holder->GetTag(),Collider::TAG::FOOT,Collider::TAG::SPIN});

	geo = std::make_unique<Sphere>(trans_.pos, movedPos_, SEARCH_RADIUS);
	MakeCollider(Collider::TAG::SEARCH, std::move(geo), { holder->GetTag(),Collider::TAG::FOOT,Collider::TAG::SPIN,Collider::TAG::GROUND,Collider::TAG::NORMAL_OBJECT});
	
	trans_.Update();

	Update();
}

void CannonShot::Update(void)
{
	//更新
	update_[state_]();
}

void CannonShot::Draw(void)
{
	//描画
	draw_[state_]();
}

void CannonShot::OnHit(const std::weak_ptr<Collider> _hitCol)
{
	//所持者
	const auto& holder = holder_.lock();
	
	if (holder->GetTag() == Collider::TAG::PLAYER1
		|| holder->GetTag() == Collider::TAG::PLAYER2
		|| holder->GetTag() == Collider::TAG::PLAYER3
		|| holder->GetTag() == Collider::TAG::PLAYER4
		|| holder->GetTag() == Collider::TAG::MACHINE
		)
	{
		if (collider_[static_cast<int>(COL::SEARCH)]->IsHit())
		{
			//標的に対する移動ベクトル
			VECTOR moveVecToTarget = Utility::GetMoveVec(movedPos_, holder->GetOwner().GetTrans().pos);
			
			//下方向には補正しない
			//moveVecToTarget.y = 0.0f;

			//標的に少し傾ける
			movePowToTarget_ = VAdd(movePow_,VScale(moveVecToTarget,speed_* SEARCH_MOVE_POW_MULTI));
		}
		else
		{
			//爆発
			state_ = STATE::BLAST;

			//カウンタ
			aliveCnt_ = 0.0f;

			//当たり判定増大
			auto& sphere = dynamic_cast<Sphere&>(collider_[static_cast<int>(COL::MAIN)]->GetGeometry());
			sphere.SetRadius(BLAST_RADIUS);
		}
	}
	else if (holder->GetTag() == Collider::TAG::NORMAL_OBJECT
		|| holder->GetTag() == Collider::TAG::GROUND)
	{
		if (!collider_[static_cast<int>(COL::MAIN)]->IsHit())return;

		//爆発
		state_ = STATE::BLAST;

		//カウンタ
		aliveCnt_ = 0.0f;

		//当たり判定増大
		auto& sphere = dynamic_cast<Sphere&>(collider_[static_cast<int>(COL::MAIN)]->GetGeometry());
		sphere.SetRadius(BLAST_RADIUS);
	}
}

void CannonShot::UpdateAlive(void)
{
	//デルタタイム
	const float delta = SceneManager::GetInstance().GetDeltaTime();

	trans_.pos = movedPos_;

	//カウンタ
	aliveCnt_ += delta;

	//生存時間制限
	if (aliveCnt_ > ALIVE_TIME)
	{
		//爆発
		state_ = STATE::BLAST;

		//カウンタ初期化
		aliveCnt_ = 0.0f;

		//当たり判定増大
		auto& sphere = dynamic_cast<Sphere&>(collider_[static_cast<int>(COL::MAIN)]->GetGeometry());
		sphere.SetRadius(BLAST_RADIUS);

		return;
	}

	//移動力
	GravityManager::GetInstance().CalcGravity(Utility::DIR_D, gravPow_,200.0f);
	movePow_ = trans_.quaRot.PosAxis(VGet(0.0f, gravPow_.y, speed_));

	//移動
	movedPos_ = VAdd(movedPos_, movePow_);
	movedPos_ = VAdd(movedPos_, movePowToTarget_);

	//方向
	trans_.quaRot = Quaternion::LookRotation(Utility::GetMoveVec(trans_.pos, VAdd(movedPos_, movePow_)));

	//モデル更新
	trans_.Update();
}

void CannonShot::UpdateBlast(void)
{
	//デルタタイム
	const float delta = SceneManager::GetInstance().GetDeltaTime();

	//カウンタ
	blastCnt_ += delta;

	if (blastCnt_ > BLAST_TIME)
	{
		//死亡
		state_ = STATE::DEAD;

		return;
	}
}

void CannonShot::UpdateDead(void)
{
}

void CannonShot::DrawAlive(void)
{
	//モデル描画
	MV1DrawModel(trans_.modelId);

	//for (auto& col : collider_)
	//{
	//	col->GetGeometry().Draw();
	//}
}

void CannonShot::DrawBlast(void)
{
	//for (auto& col : collider_)
	//{
	//	col->GetGeometry().Draw();
	//}
}

void CannonShot::DrawDead(void)
{
}
