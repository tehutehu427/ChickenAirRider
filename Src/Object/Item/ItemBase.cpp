#include "../pch.h"
#include "../Manager/System/ResourceManager.h"
#include "../Manager/System/SceneManager.h"
#include "../Manager/Game/ItemManager.h"
#include "../Object/Common/Geometry/Sphere.h"
#include "ItemBase.h"

ItemBase::ItemBase(const VECTOR _pos, const Parameter& _param, const int _imageId)
{
	trans_.pos = _pos;
	param_ = _param;
	trans_.modelId = _imageId;
	displayCnt_ = 0.0f;
	state_ = STATE::ALIVE;
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
	//コライダ生成
	std::unique_ptr<Geometry> geo = std::make_unique<Sphere>(trans_.pos, trans_.pos, RADIUS);
	MakeCollider(Collider::TAG::POWER_UP, std::move(geo));
}

void ItemBase::Update(void)
{
	//死亡ならスキップ
	if (state_ == STATE::DEAD)return;

	//取得なら表示
	else if (state_ == STATE::GOT)
	{
		//カウンタ
		const auto& delta = SceneManager::GetInstance().GetDeltaTime();
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
}
