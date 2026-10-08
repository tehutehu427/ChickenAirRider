#include "../pch.h"
#include "../Utility/Utility.h"
#include "../Manager/System/SceneManager.h"
#include "../Manager/System/ResourceManager.h"
#include"../Manager/Game/GravityManager.h"
#include "../../Common/EffectController.h"
#include "../../Common/Geometry/Sphere.h"
#include "../../Player/Player.h"
#include "CannonShot.h"

CannonShot::CannonShot(const VECTOR& _pos, const Quaternion& _rot, const VECTOR& _scl, const Collider* _holder, const float _speed)
{
	movedPos_ = _pos;
	trans_.pos = _pos;
	trans_.quaRot = _rot;
	trans_.scl = VScale(_scl, 0.3f);
	holder_ = _holder;
	speed_ = _speed + SPEED;
	gravPow_ = Utility::VECTOR_ZERO;
	movePow_ = Utility::VECTOR_ZERO;
	aliveCnt_ = 0.0f;
	blastCnt_ = 0.0f;
	state_ = STATE::ALIVE;
	attack_ = 0.0f;

	update_[static_cast<int>(STATE::ALIVE)] = &CannonShot::UpdateAlive;
	update_[static_cast<int>(STATE::BLAST)] = &CannonShot::UpdateBlast;
	update_[static_cast<int>(STATE::DEAD)] = &CannonShot::UpdateDead;

	draw_[static_cast<int>(STATE::ALIVE)] = &CannonShot::DrawAlive;
	draw_[static_cast<int>(STATE::BLAST)] = &CannonShot::DrawBlast;
	draw_[static_cast<int>(STATE::DEAD)] = &CannonShot::DrawDead;

	changeState_[static_cast<int>(STATE::ALIVE)] = &CannonShot::ChangeStateAlive;
	changeState_[static_cast<int>(STATE::BLAST)] = &CannonShot::ChangeStateBlast;
	changeState_[static_cast<int>(STATE::DEAD)] = &CannonShot::ChangeStateDead;
}

CannonShot::~CannonShot(void)
{
}

void CannonShot::Load(void)
{
	//リソース
	auto& resMng = ResourceManager::GetInstance();

	//モデル
	trans_.modelId = resMng.LoadModelDuplicate(ResourceManager::SRC::CANNON_SHOT_MODEL);

	//エフェクト
	int id = resMng.Load(ResourceManager::SRC::BLAST_EFFECT).handleId_;
	effect_->Add(id,EffectController::EFF_TYPE::BLAST);
}

void CannonShot::Init(void)
{
	//初期化
	aliveCnt_ = 0.0f;
	blastCnt_ = 0.0f;
	state_ = STATE::ALIVE;
	gravPow_ = Utility::VECTOR_ZERO;
	movePow_ = Utility::VECTOR_ZERO;

	//所持者
	const auto& holder = holder_;
	const auto& tag = holder->GetTag();

	//攻撃力
	attack_ = dynamic_cast<const Player&>(holder->GetOwner()).GetAttack() * ATTACK_MULTI;

	//コライダ
	std::unique_ptr<Geometry> geo = std::make_unique<Sphere>(trans_.pos, movedPos_, BROUD_RADIUS, SHOT_RADIUS);
	MakeCollider(Collider::TAG::CANNON_SHOT, std::move(geo), { tag,Collider::TAG::FOOT,Collider::TAG::SPIN});

	//索敵判定
	geo = std::make_unique<Sphere>(trans_.pos, movedPos_, BROUD_RADIUS, SEARCH_RADIUS);
	MakeCollider(Collider::TAG::SEARCH, std::move(geo), { tag,Collider::TAG::FOOT,Collider::TAG::SPIN,Collider::TAG::GROUND,Collider::TAG::NORMAL_OBJECT});
	
	//モデル更新
	trans_.Update();

	//初期更新
	Update();
}

void CannonShot::Update(void)
{
	//更新
	(this->*update_[static_cast<int>(state_)])();

	//エフェクト更新
	effect_->Update();
}

void CannonShot::Draw(void)
{
	//描画
	(this->*draw_[static_cast<int>(state_)])();
}

void CannonShot::OnHit(const Collider* _hitCol)
{
	//所有者がいないなら何もしない
	if (!holder_)return;

	//生きていないなら何もしない
	if (state_ != STATE::ALIVE)return;

	//所持者
	const auto& holder = holder_;
	const auto& hiter = _hitCol;
	
	if (hiter->IsIncludeMyTag({Collider::TAG::PLAYER1, Collider::TAG::PLAYER2, Collider::TAG::PLAYER3, Collider::TAG::PLAYER4, Collider::TAG::MACHINE}))
	{
		//感知判定に当たっているなら追尾
		if (collider_[static_cast<int>(COL::SEARCH)]->IsHit())
		{
			//標的に対する移動ベクトル
			VECTOR moveVecToTarget = Utility::GetMoveVec(movedPos_, hiter->GetOwner().GetTrans().pos);
			
			//標的に少し傾ける
			movePowToTarget_ = VAdd(movePow_,VScale(moveVecToTarget,speed_* SEARCH_MOVE_POW_MULTI));
		}
		//本体が当たったので爆発
		else
		{
			//爆発
			(this->*changeState_[static_cast<int>(STATE::BLAST)])();
		}
	}
	else if (hiter->IsIncludeMyTag({Collider::TAG::NORMAL_OBJECT, Collider::TAG::GROUND}))
	{
		//メイン判定にあたってない　または　すでに爆発しているなら何もしない
		if (!collider_[static_cast<int>(COL::MAIN)]->IsHit() || state_ == STATE::BLAST)return;

		//爆発
		(this->*changeState_[static_cast<int>(STATE::BLAST)])();
	}
}

void CannonShot::UpdateAlive(void)
{
	//デルタタイム
	const float delta = SceneManager::GetInstance().GetDeltaTime();

	//座標更新
	trans_.pos = movedPos_;

	//カウンタ
	aliveCnt_ += delta;

	//生存時間制限
	if (aliveCnt_ > ALIVE_TIME)
	{
		//爆発
		(this->*changeState_[static_cast<int>(STATE::BLAST)])();

		return;
	}

	//移動力
	GravityManager::GetInstance().CalcGravity(Utility::DIR_D, gravPow_, GRAVITY_POW);
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
		(this->*changeState_[static_cast<int>(STATE::DEAD)])();
	}
}

void CannonShot::UpdateDead(void)
{
}

void CannonShot::DrawAlive(void)
{
	//モデル描画
	MV1DrawModel(trans_.modelId);
}

void CannonShot::DrawBlast(void)
{
}

void CannonShot::DrawDead(void)
{
}

void CannonShot::ChangeStateAlive(void)
{
	//生存
	state_ = STATE::ALIVE;
}

void CannonShot::ChangeStateBlast(void)
{
	//爆発
	state_ = STATE::BLAST;

	//当たり判定増大
	auto& sphere = dynamic_cast<Sphere&>(collider_[static_cast<int>(COL::MAIN)]->GetGeometry());
	sphere.SetRadius(BLAST_RADIUS);

	//索敵範囲は無効化
	SetIsEnabledByTag(Collider::TAG::SEARCH, false);

	//爆発エフェクト
	effect_->Play(EffectController::EFF_TYPE::BLAST, trans_.pos, Utility::VECTOR_ZERO, BLAST_EFFECT_SIZE);
}

void CannonShot::ChangeStateDead(void)
{
	//死亡
	state_ = STATE::DEAD;

	//当たり判定無効化
	SetIsEnabledByAll(false);
}
