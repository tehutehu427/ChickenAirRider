#include "../pch.h"
#include "../Player.h"
#include "../Object/Common/Geometry/Line.h"
#include "../Object/Common/Geometry/Sphere.h"
#include "../Object/Common/Geometry/Model.h"
#include "../Manager/System/ResourceManager.h"
#include "../Manager/System/SoundManager.h"
#include "../Manager/Game/MachineManager.h"
#include "../Parameter/Parameter.h"
#include "../Object/Item/ItemBase.h"
#include "../Object/Item/PowerUpItemBase.h"
#include "../Object/Item/BattleItem/CannonShot.h"
#include "../Action/ActionBase.h"
#include "PlayerOnHit.h"

PlayerOnHit::PlayerOnHit(Player& _player)
	: player_(_player)
{
	//タグごとのヒット処理格納
	onHit_[static_cast<int>(Collider::TAG::PLAYER1)] = nullptr;
	onHit_[static_cast<int>(Collider::TAG::PLAYER2)] = nullptr;
	onHit_[static_cast<int>(Collider::TAG::PLAYER3)] = nullptr;
	onHit_[static_cast<int>(Collider::TAG::PLAYER4)] = nullptr;
	onHit_[static_cast<int>(Collider::TAG::NORMAL_OBJECT)] = &PlayerOnHit::NormalObjectOnHit;
	onHit_[static_cast<int>(Collider::TAG::TREE)] = &PlayerOnHit::NormalObjectOnHit;
	onHit_[static_cast<int>(Collider::TAG::GROUND)] = &PlayerOnHit::GroundOnHit;
	onHit_[static_cast<int>(Collider::TAG::MACHINE)] = nullptr;
	onHit_[static_cast<int>(Collider::TAG::MACHINE_RIDE)] = &PlayerOnHit::RideMachineOnHit;
	onHit_[static_cast<int>(Collider::TAG::ITEM_BOX)] = &PlayerOnHit::NormalObjectOnHit;
	onHit_[static_cast<int>(Collider::TAG::POWER_UP)] = &PlayerOnHit::PowerUpItemOnHit;
	onHit_[static_cast<int>(Collider::TAG::BATTLE_ITEM)] = &PlayerOnHit::BattleItemOnHit;
	onHit_[static_cast<int>(Collider::TAG::SPIN)] = &PlayerOnHit::SpinOnHit;
	onHit_[static_cast<int>(Collider::TAG::CANNON_SHOT)] = &PlayerOnHit::CannonShotOnHit;
	onHit_[static_cast<int>(Collider::TAG::SEARCH)] = nullptr;
	onHit_[static_cast<int>(Collider::TAG::WORLD_BORDER)] = &PlayerOnHit::NormalObjectOnHit;
	onHit_[static_cast<int>(Collider::TAG::GLIDER_BORDER)] = &PlayerOnHit::NormalObjectOnHit;
	onHit_[static_cast<int>(Collider::TAG::GLIDE_STAGE)] = &PlayerOnHit::GlideStageOnHit;
}

PlayerOnHit::~PlayerOnHit(void)
{
}

void PlayerOnHit::Load(void)
{
}

void PlayerOnHit::OnHit(const Collider* _hitCol)
{
	//タグ
	const auto& hitTag = _hitCol->GetTag();

	//タグごとのヒット処理
	if (onHit_[static_cast<int>(hitTag)] != nullptr)
	{
		(this->*onHit_[static_cast<int>(hitTag)])(_hitCol);
	}
}

void PlayerOnHit::NormalObjectOnHit(const Collider* _hitCol)
{
	//相手コライダ
	const auto& hitCol = _hitCol;

	//当たった形状情報
	const auto& hitGeo = hitCol->GetGeometry();

	//コライダ
	auto& mainCol = player_.GetColliders()[static_cast<int>(Player::COL_VALUE::MAIN)];
	auto& groundPreCol = player_.GetColliders()[static_cast<int>(Player::COL_VALUE::GROUNDED)];

	//位置の補正
	const auto& hit = mainCol->GetGeometry().GetHitResult();

	//移動量
	VECTOR movePow = VSub(player_.GetMovedPos(), player_.GetTrans().pos);

	//接触地点にまで戻す
	player_.SetMovedPos(VAdd(player_.GetMovedPos(),VScale(movePow, hit.t)));

	// 少し押し戻す（めり込み回避）
	player_.SetMovedPos(VAdd(player_.GetMovedPos(), VScale(hit.normal, 0.007f)));

	//移動量分押し戻す
	player_.SetMovedPos(VAdd(player_.GetMovedPos(),VScale(movePow, -1.0f)));

	// 残り移動をスライド方向へ
	float remain = 1.0f - hit.t;
	VECTOR slide = VSub(movePow, VScale(hit.normal, VDot(movePow, hit.normal)));
	player_.SetMovedPos(VAdd(player_.GetMovedPos(), VScale(slide, remain)));

	//接地しているか
	if (groundPreCol->IsHit() && player_.GetAction().IsHit())
	{
		player_.GetAction().ResetAxisX();
		player_.SetIsGrounded(true);
	}
}

void PlayerOnHit::GroundOnHit(const Collider* _hitCol)
{
	//各コライダ
	auto& mainCol = player_.GetColliders()[static_cast<int>(Player::COL_VALUE::MAIN)];
	auto& groundPreCol = player_.GetColliders()[static_cast<int>(Player::COL_VALUE::GROUNDED)];

	//相手コライダ
	const auto& hitCol = _hitCol;

	//相手モデル
	Model& model = dynamic_cast<Model&>(hitCol->GetGeometry());
	const int hitNum = model.GetHitInfo().HitNum;

	//自身の線
	Line& line = dynamic_cast<Line&>(groundPreCol->GetGeometry());

	//自身の球
	Sphere& mainSphere = dynamic_cast<Sphere&>(mainCol->GetGeometry());
	float radius = mainSphere.GetRadius();

	//移動後座標
	VECTOR pos = player_.GetMovedPos();
	VECTOR totalNormal = VGet(0, 0, 0);
	float maxDepth = 0.0f;

	// grounded の判定は最後に行う
	bool groundedThisFrame = false;

	//足元のみ判定
	if (groundPreCol->IsHit())
	{
		//当たった座標
		VECTOR hitPos = line.GetHitInfo().HitPosition;
		
		//法線
		VECTOR normal = VNorm(line.GetHitInfo().Normal);

		//深度
		float depth = radius - VDot(normal, VSub(pos, hitPos));

		//めり込んでるなら
		if (depth > 0.0f)
		{
			//保存
			totalNormal = VAdd(totalNormal, normal);
			maxDepth = std::max(maxDepth, depth);

			//接地判定
			groundedThisFrame = true;

			//回転を元に戻す
			player_.GetAction().ResetAxisX();
		}
	}

	//押し戻し
	if (maxDepth > 0.0f)
	{
		VECTOR N = VNorm(totalNormal);
		pos = VAdd(pos, VScale(N, maxDepth));
	}

	//移動後座標の更新
	player_.SetMovedPos(pos);

	//接地判定の更新
	player_.SetIsGrounded(groundedThisFrame);
    
	//前座標の更新
    player_.SetPrePos(pos);
}

void PlayerOnHit::RideMachineOnHit(const Collider* _hitCol)
{
	//機体に乗っているなら処理なし
	if (player_.GetState() == Player::STATE::RIDE_MACHINE)return;

	//スペシャルボタンを押し続けた
	if (!player_.GetLogic().IsGetOff())return;

	//相手コライダ
	const auto& hitCol = _hitCol;

	//機体確定なので型変換
	const Machine& machine = dynamic_cast<const Machine&>(hitCol->GetOwner());

	//機体管理
	auto& machineMng = MachineManager::GetInstance();

	//機体に移動
	player_.SetMovedPos(machine.GetTrans().pos);

	//機体を比較し取得
	player_.RideMachine(std::move(machineMng.GetMachine(machine)));
}

void PlayerOnHit::PowerUpItemOnHit(const Collider* _hitCol)
{
	//身体が当たっていないならスキップ
	if (!player_.GetColliders()[static_cast<int>(Player::COL_VALUE::MAIN)]->IsHit())return;

	//相手コライダ
	const auto& hitCol = _hitCol;

	//消失済みならスキップ
	if (hitCol->IsDead())return;

	//SE
	SoundManager::GetInstance().Play(SoundManager::SOUND_NAME::GET_ITEM, SoundManager::PLAYTYPE::BACK);

	//対象パワーアップアイテム
	const auto& powerUpItem = dynamic_cast<const PowerUpItemBase&>(hitCol->GetOwner());

	//パラメーター
	Parameter param = powerUpItem.GetParam();

	//プレイヤーに加算
	player_.SetParam(player_.GetParam() + param);
}

void PlayerOnHit::BattleItemOnHit(const Collider* _hitCol)
{
}

void PlayerOnHit::SpinOnHit(const Collider* _hitCol)
{
	//無敵中なら処理しない
	if (!player_.IsEndInvincible() || player_.GetState() != Player::STATE::RIDE_MACHINE)return;

	//相手コライダ
	const auto& hitCol = _hitCol;

	//SE
	SoundManager::GetInstance().Play(SoundManager::SOUND_NAME::DAMAGE, SoundManager::PLAYTYPE::BACK);

	//スピンの相手
	const auto& spinParent = dynamic_cast<const Player&>(hitCol->GetOwner());

	//機体に乗っていないなら処理しない
	if(!spinParent.GetMachine())return;

	//攻撃力
	float attack = spinParent.GetAttack();

	//ダメージ処理
	player_.Damage(attack);

	//無敵時間リセット
	player_.SetInvincible(INVINCIBLE_SPIN);
}

void PlayerOnHit::CannonShotOnHit(const Collider* _hitCol)
{
	//無敵中なら処理しない
	if (!player_.IsEndInvincible() || player_.GetState() != Player::STATE::RIDE_MACHINE)return;

	//相手コライダ
	const auto& hitCol = _hitCol;

	//SE
	SoundManager::GetInstance().Play(SoundManager::SOUND_NAME::DAMAGE, SoundManager::PLAYTYPE::BACK);

	//大砲の弾
	const auto& shot = dynamic_cast<const CannonShot&>(hitCol->GetOwner());

	//攻撃力
	float attack = shot.GetAttack();

	//ダメージ処理
	player_.Damage(attack);

	//無敵時間リセット
	player_.SetInvincible(CannonShot::INVINCIBLE);
}

void PlayerOnHit::GlideStageOnHit(const Collider* _hitCol)
{
	//既に移動終了なら判定しない
	if (!player_.GetCanMove())return;

	//足元が当たったなら
	if (player_.GetColliders()[static_cast<int>(Player::COL_VALUE::GROUNDED)]->IsHit())
	{
		//移動できないようにする
		player_.SetCanMove(false);
	}
}
