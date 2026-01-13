#include "../pch.h"
#include "../Manager/System/SceneManager.h"
#include "../Manager/Game/GravityManager.h"
#include "../Utility/Utility.h"
#include "../Object/Common/Geometry/Sphere.h"
#include "BattleItemBase.h"

BattleItemBase::BattleItemBase(const VECTOR& _pos, const VECTOR& _vec, const int _imageId, const int _modelId, const BATTLE_ITEM_TYPE _battleItemType)
	: ItemBase(_pos,_vec)
{
	imgId_ = _imageId;
	trans_.modelId = _modelId;
	battleItemType_ = _battleItemType;
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
		if (displayCnt_ > 1.0f)
		{
			//死亡
			state_ = STATE::DEAD;
		}
	}
}

void BattleItemBase::Draw(void)
{
}

void BattleItemBase::OnHit(std::weak_ptr<Collider> _hitCol)
{
}
