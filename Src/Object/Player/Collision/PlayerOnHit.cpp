#include "../pch.h"
#include "../Player.h"
#include "../Object/Common/Geometry/Line.h"
#include "../Object/Common/Geometry/Sphere.h"
#include "../Object/Common/Geometry/Model.h"
#include "../Manager/Game/MachineManager.h"
#include "../Parameter/Parameter.h"
#include "../Object/Item/ItemBase.h"
#include "../Action/ActionBase.h"
#include "PlayerOnHit.h"

PlayerOnHit::PlayerOnHit(Player& _player, Transform& _trans)
	: player_(_player),
	playerTrans_(_trans)
{
	//タグごとのヒット処理格納
	onHit_[Collider::TAG::NORMAL_OBJECT] = [this](const std::weak_ptr<Collider> _hitCol) {NormalObjectOnHit(_hitCol); };
	onHit_[Collider::TAG::GROUND] = [this](const std::weak_ptr<Collider> _hitCol) {NormalObjectOnHit(_hitCol); };
	onHit_[Collider::TAG::MACHINE] = [this](const std::weak_ptr<Collider> _hitCol) {NormalObjectOnHit(_hitCol); };
	onHit_[Collider::TAG::MACHINE_RIDE] = [this](const std::weak_ptr<Collider> _hitCol) {RideMachineOnHit(_hitCol); };
	onHit_[Collider::TAG::ITEM_BOX] = [this](const std::weak_ptr<Collider> _hitCol) {NormalObjectOnHit(_hitCol); };
	onHit_[Collider::TAG::POWER_UP] = [this](const std::weak_ptr<Collider> _hitCol) {PowerUpItemOnHit(_hitCol); };
}

PlayerOnHit::~PlayerOnHit(void)
{
}

void PlayerOnHit::OnHit(const std::weak_ptr<Collider> _hitCol)
{
	//タグ
	const auto& hitTag = _hitCol.lock()->GetTag();

	//タグごとのヒット処理
	onHit_[hitTag](_hitCol);
}

void PlayerOnHit::NormalObjectOnHit(const std::weak_ptr<Collider> _hitCol)
{
	//const auto& hitGeo = _hitCol.lock()->GetGeometry();

	//コライダ
	auto& mainCol = player_.GetColliders()[static_cast<int>(Player::COL_VALUE::MAIN)];
	auto& groundPreCol = player_.GetColliders()[static_cast<int>(Player::COL_VALUE::GROUNDED_PRE)];
	////auto& groundOldCol = player_.GetColliders()[static_cast<int>(Player::COL_VALUE::GROUNDED_OLD)];

	////位置の補正
	//const auto& hit = mainCol->GetGeometry().GetHitResult();

	////移動前から移動後までのベクトル
	//VECTOR v = VSub(player_.GetMovedPos(), player_.GetTrans().pos);

	//// 衝突時位置まで移動
	//player_.SetMovedPos(VAdd(player_.GetTrans().pos, VScale(v, hit.t)));

	//// 少し押し戻す（めり込み回避）
	//player_.SetMovedPos(VAdd(player_.GetMovedPos(), VScale(hit.normal, 0.007f)));

	//// 残り移動をスライド方向へ
	//float remain = 1.0f - hit.t;
	//VECTOR slide = VSub(v, VScale(hit.normal, VDot(v, hit.normal)));
	//player_.SetMovedPos(VAdd(player_.GetMovedPos(), VScale(slide, remain)));

	////接地しているか
	//if (groundPreCol->IsHit() && player_.GetAction().IsHit())
	//{
	//	player_.GetAction().ResetAxisX();
	//	player_.SetIsGrounded(true);
	//}

	//モデル
	Model& model = dynamic_cast<Model&>(_hitCol.lock()->GetGeometry());
	const int hitNum = model.GetHitInfo().HitNum;

	//球
	Sphere& mainSphere = dynamic_cast<Sphere&>(mainCol->GetGeometry());
	
	//半径
	float radius = mainSphere.GetRadius();

	if (mainCol->IsHit())
	{
		for (int i = 0; i < hitNum; i++)
		{
			//当たった箇所
			auto hit = model.GetHitInfo().Dim[i];
			VECTOR hitPos = hit.HitPosition;

			//三角メッシュとの判定
			int pHit = HitCheck_Sphere_Triangle(playerTrans_.pos, radius, hit.Position[0], hit.Position[1], hit.Position[2]);

			//当たっているなら
			if (pHit)
			{
				//法線
				VECTOR normal = hit.Normal;

				//押し出し
				player_.SetMovedPos(VAdd(player_.GetMovedPos(), VScale(normal, 3.0f)));
				playerTrans_.pos = player_.GetMovedPos();
				playerTrans_.Update();
			}
		}
	}
}

void PlayerOnHit::RideMachineOnHit(const std::weak_ptr<Collider> _hitCol)
{
	//機体に乗っているなら処理なし
	if (player_.GetState() == Player::STATE::RIDE_MACHINE)return;

	//スペシャルボタンを押し続けた
	if (!player_.GetLogic().IsGetOff())return;

	//機体確定なので型変換
	const Machine& machine = dynamic_cast<const Machine&>(_hitCol.lock()->GetParent());

	//機体管理
	auto& machineMng = MachineManager::GetInstance();

	//機体に移動
	player_.SetMovedPos(machine.GetTrans().pos);

	//機体を比較し取得
	player_.RideMachine(std::move(machineMng.GetMachine(machine)));
}

void PlayerOnHit::PowerUpItemOnHit(const std::weak_ptr<Collider> _hitCol)
{
	//対象パワーアップアイテム
	const auto& powerUpItem = dynamic_cast<const ItemBase&>(_hitCol.lock()->GetParent());

	//パラメーター
	Parameter param = powerUpItem.GetParam();

	//プレイヤーに加算
	player_.SetParam(player_.GetParam() + param);
}
