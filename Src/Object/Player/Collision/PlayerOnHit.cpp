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

PlayerOnHit::PlayerOnHit(Player& _player, Transform& _trans)
	: player_(_player),
	playerTrans_(_trans)
{
	//タグごとのヒット処理格納
	onHit_[Collider::TAG::PLAYER1] = [this](const Collider& _hitCol) {};
	onHit_[Collider::TAG::PLAYER2] = [this](const Collider& _hitCol) {};
	onHit_[Collider::TAG::PLAYER3] = [this](const Collider& _hitCol) {};
	onHit_[Collider::TAG::PLAYER4] = [this](const Collider& _hitCol) {};
	onHit_[Collider::TAG::NORMAL_OBJECT] = [this](const Collider& _hitCol) {NormalObjectOnHit(_hitCol); };
	onHit_[Collider::TAG::TREE] = [this](const Collider& _hitCol) {NormalObjectOnHit(_hitCol); };
	onHit_[Collider::TAG::GROUND] = [this](const Collider& _hitCol) {GroundOnHit(_hitCol); };
	onHit_[Collider::TAG::MACHINE] = [this](const Collider& _hitCol) {/*NormalObjectOnHit(_hitCol); */};
	onHit_[Collider::TAG::MACHINE_RIDE] = [this](const Collider& _hitCol) {RideMachineOnHit(_hitCol); };
	onHit_[Collider::TAG::ITEM_BOX] = [this](const Collider& _hitCol) {NormalObjectOnHit(_hitCol); };
	onHit_[Collider::TAG::POWER_UP] = [this](const Collider& _hitCol) {PowerUpItemOnHit(_hitCol); };
	onHit_[Collider::TAG::BATTLE_ITEM] = [this](const Collider& _hitCol) {BattleItemOnHit(_hitCol); };
	onHit_[Collider::TAG::SPIN] = [this](const Collider& _hitCol) { SpinOnHit(_hitCol); };
	onHit_[Collider::TAG::CANNON_SHOT] = [this](const Collider& _hitCol) { CannonShotOnHit(_hitCol); };
	onHit_[Collider::TAG::SEARCH] = [this](const Collider& _hitCol) {};
	onHit_[Collider::TAG::WORLD_BORDER] = [this](const Collider& _hitCol) {NormalObjectOnHit(_hitCol); };
	onHit_[Collider::TAG::GLIDER_BORDER] = [this](const Collider& _hitCol) {NormalObjectOnHit(_hitCol); };
	onHit_[Collider::TAG::GLIDE_STAGE] = [this](const Collider& _hitCol) {GlideStageOnHit(_hitCol); };
}

PlayerOnHit::~PlayerOnHit(void)
{
}

void PlayerOnHit::Load(void)
{
	//インスタンス
	auto& res = ResourceManager::GetInstance();
	auto& snd = SoundManager::GetInstance();

	//アイテムゲット
	int id = res.Load(ResourceManager::SRC::GET_ITEM).handleId_;
	snd.Add(SoundManager::SOUND_NAME::GET_ITEM, id, SoundManager::TYPE::SE);

	//ダメージ
	id = res.Load(ResourceManager::SRC::DAMAGE).handleId_;
	snd.Add(SoundManager::SOUND_NAME::DAMAGE, id, SoundManager::TYPE::SE);
}

void PlayerOnHit::OnHit(const Collider& _hitCol)
{
	//タグ
	const auto& hitTag = _hitCol.GetTag();

	//タグごとのヒット処理
	onHit_[hitTag](_hitCol);
}

void PlayerOnHit::NormalObjectOnHit(const Collider& _hitCol)
{
	//当たった形状情報
	const auto& hitGeo = _hitCol.GetGeometry();

	//コライダ
	auto& mainCol = player_.GetColliders()[static_cast<int>(Player::COL_VALUE::MAIN)];
	auto& groundPreCol = player_.GetColliders()[static_cast<int>(Player::COL_VALUE::GROUNDED)];
	//auto& groundOldCol = player_.GetColliders()[static_cast<int>(Player::COL_VALUE::GROUNDED_OLD)];

	//位置の補正
	const auto& hit = mainCol->GetGeometry().GetHitResult();

	//移動量
	VECTOR movePow = VSub(player_.GetMovedPos(), playerTrans_.pos);

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
	if (groundPreCol->IsHit() && player_.GetAction().lock()->IsHit())
	{
		player_.GetAction().lock()->ResetAxisX();
		player_.SetIsGrounded(true);
	}
}

void PlayerOnHit::GroundOnHit(const Collider& _hitCol)
{
	//各コライダ
	auto& mainCol = player_.GetColliders()[static_cast<int>(Player::COL_VALUE::MAIN)];
	auto& groundPreCol = player_.GetColliders()[static_cast<int>(Player::COL_VALUE::GROUNDED)];

	//相手モデル
	Model& model = dynamic_cast<Model&>(_hitCol.GetGeometry());
	const int hitNum = model.GetHitInfo().HitNum;

	//自身の線
	Line& line = dynamic_cast<Line&>(groundPreCol->GetGeometry());
	//float linePoint = line.GetLocalPosPoint1().y + line.GetLocalPosPoint2().y + 1.0f;

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
		
		//足元の長さ
		//float footLength = fabsf(line.GetLocalPosPoint2().y - line.GetLocalPosPoint1().y) - FOOT_COMP;

		//深度
		float depth = radius - VDot(normal, VSub(pos, hitPos));

		//めり込んでるなら
		if (depth > 0.0f)
		{
			//保存
			totalNormal = VAdd(totalNormal, normal);
			maxDepth = std::max(maxDepth, depth);

			// 接地判定
			groundedThisFrame = true;

			// 回転を元に戻す
			player_.GetAction().lock()->ResetAxisX();
		}
	}

	// 押し戻し
	if (maxDepth > 0.0f)
	{
		VECTOR N = VNorm(totalNormal);
		pos = VAdd(pos, VScale(N, maxDepth));
	}

	player_.SetMovedPos(pos);

	// ---- grounded の更新は最後に1回だけ ----
	player_.SetIsGrounded(groundedThisFrame);
    // ---- 最後に前フレームの位置を保存（次フレーム用） ----
    player_.SetPrePos(pos);
}

void PlayerOnHit::RideMachineOnHit(const Collider& _hitCol)
{
	//機体に乗っているなら処理なし
	if (player_.GetState() == Player::STATE::RIDE_MACHINE)return;

	//スペシャルボタンを押し続けた
	if (!player_.GetLogic().IsGetOff())return;

	//機体確定なので型変換
	const Machine& machine = dynamic_cast<const Machine&>(*_hitCol.GetOwner());

	//機体管理
	auto& machineMng = MachineManager::GetInstance();

	//機体に移動
	player_.SetMovedPos(machine.GetTrans().pos);

	//機体を比較し取得
	player_.RideMachine(std::move(machineMng.GetMachine(machine)));
}

void PlayerOnHit::PowerUpItemOnHit(const Collider& _hitCol)
{
	//身体が当たっていないならスキップ
	if (!player_.GetColliders()[static_cast<int>(Player::COL_VALUE::MAIN)]->IsHit())return;

	//消失済みならスキップ
	if (_hitCol.IsDead())return;

	//SE
	SoundManager::GetInstance().Play(SoundManager::SOUND_NAME::GET_ITEM, SoundManager::PLAYTYPE::BACK);

	//対象パワーアップアイテム
	const auto& powerUpItem = dynamic_cast<const PowerUpItemBase&>(*_hitCol.GetOwner());

	//パラメーター
	Parameter param = powerUpItem.GetParam();

	//プレイヤーに加算
	player_.SetParam(player_.GetParam() + param);
}

void PlayerOnHit::BattleItemOnHit(const Collider& _hitCol)
{
}

void PlayerOnHit::SpinOnHit(const Collider& _hitCol)
{
	//無敵中なら処理しない
	if (!player_.IsEndInvincible() || player_.GetState() != Player::STATE::RIDE_MACHINE)return;

	//SE
	SoundManager::GetInstance().Play(SoundManager::SOUND_NAME::DAMAGE, SoundManager::PLAYTYPE::BACK);

	//スピンの相手
	const auto& spinParent = dynamic_cast<const Player&>(*_hitCol.GetOwner());

	//攻撃力
	float attack = spinParent.GetAttack();

	//ダメージ処理
	player_.Damage(attack);

	//無敵時間リセット
	player_.SetInvincible(INVINCIBLE_SPIN);
}

void PlayerOnHit::CannonShotOnHit(const Collider& _hitCol)
{
	//無敵中なら処理しない
	if (!player_.IsEndInvincible() || player_.GetState() != Player::STATE::RIDE_MACHINE)return;

	//SE
	SoundManager::GetInstance().Play(SoundManager::SOUND_NAME::DAMAGE, SoundManager::PLAYTYPE::BACK);

	//大砲の弾
	const auto& shot = dynamic_cast<const CannonShot&>(*_hitCol.GetOwner());

	//攻撃力
	float attack = shot.GetAttack();

	//ダメージ処理
	player_.Damage(attack);

	//無敵時間リセット
	player_.SetInvincible(CannonShot::INVINCIBLE);
}

void PlayerOnHit::GlideStageOnHit(const Collider& _hitCol)
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
