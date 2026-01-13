#include "../pch.h"
#include "../Manager/System/SceneManager.h"
#include "../Manager/System/ResourceManager.h"
#include"../Manager/Game/GravityManager.h"
#include "../Utility/Utility.h"
#include "../../Common/Geometry/Sphere.h"
#include "../../Player/Player.h"
#include "CannonShot.h"

CannonShot::CannonShot(const VECTOR& _pos, const Quaternion& _rot, const VECTOR& _scl, std::weak_ptr<Collider> _holder)
{
	trans_.pos = _pos;
	trans_.quaRot = _rot;
	trans_.scl = VScale(_scl,0.5f);
	holder_ = _holder;
	gravPow_ = Utility::VECTOR_ZERO;
	movePow_ = Utility::VECTOR_ZERO;
	cnt_ = 0.0f;
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
	cnt_ = 0.0f;
	state_ = STATE::ALIVE;
	gravPow_ = Utility::VECTOR_ZERO;
	movePow_ = Utility::VECTOR_ZERO;

	//攻撃力
	attack_ = dynamic_cast<const Player&>(holder_.lock()->GetParent()).GetAttack();

	//コライダ
	std::unique_ptr<Geometry> geo = std::make_unique<Sphere>(trans_.pos, trans_.pos, RADIUS);
	MakeCollider(Collider::TAG::CANNON_SHOT, std::move(geo), { holder_.lock()->GetTag()});
	trans_.Update();
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

void CannonShot::OnHit(std::weak_ptr<Collider> _hitCol)
{
	if (_hitCol.lock()->GetTag() == Collider::TAG::NORMAL_OBJECT
		|| _hitCol.lock()->GetTag() == Collider::TAG::PLAYER1
		|| _hitCol.lock()->GetTag() == Collider::TAG::PLAYER2
		|| _hitCol.lock()->GetTag() == Collider::TAG::PLAYER3
		|| _hitCol.lock()->GetTag() == Collider::TAG::PLAYER4)
	{
		//爆発
		state_ = STATE::BLAST;

		//カウンタ
		cnt_ = 0.0f;
	}
}

void CannonShot::UpdateAlive(void)
{
	//デルタタイム
	const float delta = SceneManager::GetInstance().GetDeltaTime();

	//カウンタ
	cnt_ += delta;

	//生存時間制限
	if (cnt_ > ALIVE_TIME)
	{
		state_ = STATE::BLAST;
	}

	//移動力
	GravityManager::GetInstance().CalcGravity(Utility::DIR_D, gravPow_);
	movePow_ = trans_.quaRot.PosAxis(VGet(0.0f, gravPow_.y, SPEED));

	//移動
	trans_.pos = VAdd(trans_.pos, movePow_);

	//モデル更新
	trans_.Update();
}

void CannonShot::UpdateBlast(void)
{
	state_ = STATE::DEAD;
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
