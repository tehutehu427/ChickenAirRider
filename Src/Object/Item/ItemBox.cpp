#include "../pch.h"
#include "../Utility/Utility.h"
#include "../Manager/Game/ItemManager.h"
#include "../Manager/System/SceneManager.h"
#include "../Manager/System/ResourceManager.h"
#include "../Manager/Game/GravityManager.h"
#include "../Object/Common/Geometry/Cube.h"
#include "../Object/Common/Geometry/Line.h"
#include "../Object/Player/Player.h"
#include "ItemBox.h"

ItemBox::ItemBox(VECTOR _pos)
{
	trans_.pos = _pos;
	createPos_ = _pos;
	isDead_ = false;
	footLine_ = Quaternion();
	health_ = 0.0f;
	invincible_ = 0.0f;
}

ItemBox::~ItemBox(void)
{
}

void ItemBox::Load(void)
{
	//モデル設定
	trans_.modelId = ResourceManager::GetInstance().LoadModelDuplicate(ResourceManager::SRC::ITEM_BOX);
}

void ItemBox::Init(void)
{
	//当たり判定
	std::unique_ptr<Geometry> geo = std::make_unique<Cube>(trans_.pos, trans_.quaRot, BOX_HALF);
	MakeCollider(Collider::TAG::ITEM_BOX, std::move(geo), { Collider::TAG::FOOT });

	//足元の判定
	geo = std::make_unique<Line>(trans_.pos, footLine_, LOCAL_LINE_UP, LOCAL_LINE_DOWN);
	MakeCollider(Collider::TAG::FOOT, std::move(geo), { Collider::TAG::ITEM_BOX });

	//変数
	health_ = HEALTH_MAX;
	invincible_ = INVINCIBLE;

	//更新
	trans_.Update();
}

void ItemBox::Update(void)
{
	//死亡済みなので処理しない
	if (isDead_)return;

	//デルタタイム
	const float delta = SceneManager::GetInstance().GetDeltaTime();
	invincible_ += delta;

	//重力
	VECTOR grav = Utility::VECTOR_ZERO;
	GravityManager::GetInstance().CalcGravity(Utility::DIR_D, grav);
	trans_.pos = VAdd(trans_.pos, grav);

	//落下時の回転
	trans_.quaRot = trans_.quaRot.Mult(Quaternion::AngleAxis(Utility::Deg2RadF(FALL_ROT), Utility::AXIS_XZ));

	//モデル更新
	trans_.Update();
}

void ItemBox::Draw(void)
{
	//モデル描画
	MV1DrawModel(trans_.modelId);
}

void ItemBox::OnHit(std::weak_ptr<Collider> _hitCol)
{
	//足元
	const auto& footCol = collider_[FOOT_COL];

	const auto& hit = _hitCol.lock();
	const auto& hitTag = hit->GetTag();

	if (hitTag == Collider::TAG::SPIN && invincible_ > INVINCIBLE)
	{
		//スピンをもつのはプレイヤーのみ
		const auto& player = dynamic_cast<const Player&>(_hitCol.lock()->GetParent());

		//攻撃力分ダメージ
		health_ -= player.GetAttack();

		//無敵時間リセット
		invincible_ = 0.0f;
		
		//体力がなくなったら
		if (health_ < 0)
		{
			//コライダ削除
			collider_[0]->Kill();

			//死亡判定
			isDead_ = true;

			//アイテム生成
			ItemManager::GetInstance().CreateItem(trans_.pos);
		}
	}
	else if (hitTag == Collider::TAG::GROUND
		|| hitTag == Collider::TAG::NORMAL_OBJECT)
	{
		if (footCol->IsHit())
		{
			//押し戻し
			trans_.pos = VAdd(trans_.pos, VScale(Utility::DIR_U, 3.0f));

			//回転リセット
			trans_.quaRot = Quaternion();
		}
	}

	//モデル更新
	trans_.Update();
}
