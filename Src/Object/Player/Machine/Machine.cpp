#include"../pch.h"
#include"../../Manager/System/SceneManager.h"
#include "../Object/Common/Geometry/Sphere.h"
#include "../Object/Player/Player.h"
#include "../Object/Item/BattleItem/CannonShot.h"
#include "Machine.h"

Machine::Machine(const MachineImportData& _importData, const int _modelId, const VECTOR _pos)
{
	trans_ = Transform();
	trans_.pos = _pos;
	trans_.modelId = _modelId;
	trans_.scl = _importData.scale;
	isAnim_ = _importData.isAnim;
	radius_ = _importData.hitRadius;
	unitParam_ = _importData.param;
	damage_ = 0.0f;
	invincible_ = 0.0f;
}

Machine::~Machine(void)
{
}

void Machine::Load(void)
{
}

void Machine::Init(void)
{
	//初期化
	invincible_ = 0;

	//モデル
	trans_.localPos.y -= 20.0f;

	//アニメーション
	InitAnimation();

	//モデル更新用
	Update();
}

void Machine::Update(void)
{
	//デルタタイム
	const float delta = SceneManager::GetInstance().GetDeltaTime();
	invincible_ -= delta;

	//アニメーション
	Animation();

	//モデル更新
	trans_.Update();
}

void Machine::Draw(void)
{
	MV1DrawModel(trans_.modelId);
}

void Machine::OnHit(const std::weak_ptr<Collider> _hitCol)
{
	if (_hitCol.lock()->GetTag() == Collider::TAG::SPIN)
	{
		//攻撃者から攻撃力を取得
		const Player& player = dynamic_cast<const Player&>(_hitCol.lock()->GetParent());
		const float atk = player.GetAttack();

		//ダメージ
		damage_ += atk - (unitParam_.fixedDefence + (unitParam_.fixedWeight * Parameter::WEIGHT_AFFECT) - (unitParam_.fixedFlight * Parameter::FLIGHT_AFFECT));

		//無敵時間リセット
		invincible_ = INVINCIBLE_SPIN;
	}
	else if(_hitCol.lock()->GetTag() == Collider::TAG::CANNON_SHOT)
	{
		//攻撃者から攻撃力を取得
		const CannonShot& shot = dynamic_cast<const CannonShot&>(_hitCol.lock()->GetParent());
		const float atk = shot.GetAttack();

		//ダメージ
		damage_ += atk - (unitParam_.fixedDefence + (unitParam_.fixedWeight * Parameter::WEIGHT_AFFECT) - (unitParam_.fixedFlight * Parameter::FLIGHT_AFFECT));

		//無敵時間リセット
		invincible_ = CannonShot::INVINCIBLE;
	}

	//体力が0以下になったら
	if (IsDead())
	{
		//当たり判定削除しておく
		DeleteAllCollider();
	}
}

void Machine::CreateCol(void)
{
	//コライダ生成
	std::unique_ptr<Geometry> geo = std::make_unique<Sphere>(trans_.pos, trans_.pos, radius_);
	MakeCollider(Collider::TAG::MACHINE, std::move(geo), { Collider::TAG::MACHINE_RIDE });

	//乗り判定コライダ
	geo = std::make_unique<Sphere>(trans_.pos, trans_.pos, RIDE_COL);
	MakeCollider(Collider::TAG::MACHINE_RIDE, std::move(geo), { Collider::TAG::MACHINE,Collider::TAG::MACHINE_RIDE });
}

void Machine::DeleteCol(void)
{
	//コライダ削除
	DeleteAllCollider();
}

void Machine::SetScale(const VECTOR& _scale)
{
	VECTOR scale = _scale;
	scale.x = scale.x * MODEL_SIZE.x;
	scale.y = scale.y * MODEL_SIZE.y;
	scale.z = scale.z * MODEL_SIZE.z;

	trans_.scl = scale;
}

const bool Machine::IsDead(void) const
{
	return damage_ > unitParam_.fixedMaxHealth * Parameter::MAX_HEALTH_VALUE;
}

void Machine::InitAnimation(void)
{
	if (!isAnim_)return;

	//初期化
	anim_ = std::make_unique<AnimationController>(trans_.modelId);
	anim_->Add("Run", 0, 60.0f);
	anim_->Play("Run");
}

void Machine::Animation(void)
{
	if (!isAnim_)return;

	//更新
	anim_->Update();
}
