#include "../pch.h"
#include "../Utility/Utility.h"
#include "../Manager/Game/ItemManager.h"
#include "../Manager/System/SceneManager.h"
#include "../Manager/System/ResourceManager.h"
#include "../Manager/Game/GravityManager.h"
#include "../Manager/Game/AttackManager.h"
#include "../Object/Common/Geometry/Cube.h"
#include "../Object/Common/Geometry/Line.h"
#include "../Object/Player/Player.h"
#include "../Object/Item/BattleItem/CannonShot.h"
#include "../Renderer/ModelRenderer.h"
#include "../Renderer/ModelMaterial.h"
#include "ItemBox.h"

ItemBox::ItemBox(const VECTOR& _pos, const ITEM_TYPE _type)
{
	trans_.pos = _pos;
	createPos_ = _pos;
	crackImg_ = -1;
	type_ = _type;
	isDead_ = false;
	footLine_ = Quaternion();
	health_ = 0.0f;
	invincible_ = 0.0f;

	//色
	color_.emplace(ITEM_TYPE::POWER_UP, POWER_UP_COLOR);
	color_.emplace(ITEM_TYPE::BATTLE, BATTLE_COLOR);

	//アイテムの生成
	createItem_[static_cast<int>(ITEM_TYPE::POWER_UP)] = &ItemBox::CreatePowerUpItem;
	createItem_[static_cast<int>(ITEM_TYPE::BATTLE)]  = &ItemBox::CreateBattleItem;
}

ItemBox::~ItemBox(void)
{
}

void ItemBox::Load(void)
{
	//モデル設定
	trans_.modelId = ResourceManager::GetInstance().LoadModelDuplicate(ResourceManager::SRC::ITEM_BOX);

	//ひび画像
	crackImg_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::BOX_CRACK).handleId_;

	//マテリアル
	material_ = std::make_unique<ModelMaterial>(
		L"BoxCrackVS.cso", 0,
		L"BoxCrackPS.cso", 1
		);

	//体力の割合
	float healthRatio = 0.0f;
	if (HEALTH_MAX > 0.0f)
	{
		healthRatio	= health_ / HEALTH_MAX;
	}

	COLOR_F color = color_[type_];

	//体力
	material_->AddConstBufPS({ healthRatio,color.r,color.g,color.b });

	//ひび画像
	material_->SetTextureBuf(CRACK_IMG_BUFF, crackImg_);

	//レンダラー
	renderer_ = std::make_unique<ModelRenderer>();
}

void ItemBox::Init(void)
{
	//当たり判定
	std::unique_ptr<Geometry> geo = std::make_unique<Cube>(trans_.pos, trans_.pos, trans_.quaRot, BROUD_RADIUS, BOX_HALF);
	MakeCollider(Collider::TAG::ITEM_BOX, std::move(geo), { Collider::TAG::FOOT });

	//足元の判定
	geo = std::make_unique<Line>(trans_.pos, trans_.pos, footLine_, BROUD_RADIUS, LOCAL_LINE_UP, LOCAL_LINE_DOWN);
	MakeCollider(Collider::TAG::FOOT, std::move(geo), { Collider::TAG::ITEM_BOX });

	//変数
	health_ = HEALTH_MAX;
	invincible_ = INVINCIBLE_SPIN;

	//更新
	trans_.Update();
}

void ItemBox::Update(void)
{
	//死亡済みなので処理しない
	if (isDead_)return;

	//デルタタイム
	const float delta = SceneManager::GetInstance().GetDeltaTime();
	invincible_ -= delta;

	//重力
	VECTOR grav = Utility::VECTOR_ZERO;
	GravityManager::GetInstance().CalcGravity(Utility::DIR_D, grav, GRAVITY_POW);
	trans_.pos = VAdd(trans_.pos, grav);

	//落下時の回転
	trans_.quaRot = trans_.quaRot.Mult(Quaternion::AngleAxis(Utility::Deg2RadF(FALL_ROT), Utility::AXIS_XZ));

	//モデル更新
	trans_.Update();
}

void ItemBox::Draw(void)
{
	//死亡済みなら描画しない
	if (isDead_)return;

	//定数バッファ更新

	//体力の割合
	float healthRatio = 0.0f;
	if (HEALTH_MAX > 0.0f)
	{
		healthRatio = health_ / HEALTH_MAX;
	}

	COLOR_F color = color_[type_];
	material_->SetConstBufPS(0, { healthRatio,color.r,color.g,color.b });

	//モデル描画
	//MV1DrawModel(trans_.modelId);
	renderer_->Draw(trans_.modelId, *material_);
}

void ItemBox::OnHit(const Collider* _hitCol)
{
	//足元
	const auto& footCol = collider_[FOOT_COL];

	const auto& hitCol = _hitCol;
	const auto& hitTag = hitCol->GetTag();

	//攻撃系
	if (hitCol->IsIncludeMyTag({ Collider::TAG::SPIN,Collider::TAG::CANNON_SHOT }))
	{
		//攻撃マネージャー
		auto& atkMng = AttackManager::GetInstance();

		//自身のコライダ
		const auto& myCol = collider_[static_cast<int>(COL::MAIN)].get();

		//本体にあたってないので無視
		if (!myCol->IsHit())return;

		//当たれるか
		if (!atkMng.IsCanHit(_hitCol, myCol)) return;

		//攻撃情報
		const auto& atkData = atkMng.GetAttackData(_hitCol, myCol);

		//攻撃間隔
		if (0.0f < invincible_ && atkData.isMultiHit)
		{
			return;
		}

		//ダメージ
		health_ -= atkData.power;

		//無敵時間リセット
		invincible_ = atkData.hitInterval;
	}
	//地面系
	else if (hitCol->IsIncludeMyTag({ Collider::TAG::GROUND, Collider::TAG::NORMAL_OBJECT }))
	{
		if (footCol->IsHit())
		{
			//重力
			VECTOR grav = Utility::VECTOR_ZERO;
			GravityManager::GetInstance().CalcGravity(Utility::DIR_D, grav, GRAVITY_POW);

			//重力分押し戻し
			trans_.pos = VAdd(trans_.pos, VScale(Utility::DIR_U, -grav.y));

			//回転リセット
			trans_.quaRot = Quaternion();
		}
	}

	//体力がなくなったら
	if (health_ < 0)
	{
		//コライダ無効化
		SetIsEnabledByAll(false);

		//死亡判定
		isDead_ = true;

		//アイテム生成
		(this->*createItem_[static_cast<int>(type_)])(hitCol->GetOwner().GetTrans().pos);
	}

	//モデル更新
	trans_.Update();
}

void ItemBox::CreatePowerUpItem(const VECTOR& _pPos)const
{
	ItemManager::GetInstance().CreatePowerUpItem(trans_.pos, _pPos);
}

void ItemBox::CreateBattleItem(const VECTOR& _pPos)const
{
	ItemManager::GetInstance().CreateBattleItem(trans_.pos, _pPos);
}
