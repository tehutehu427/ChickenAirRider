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
#include "../Action/ActionBase.h"
#include "PlayerOnHit.h"

PlayerOnHit::PlayerOnHit(Player& _player, Transform& _trans)
	: player_(_player),
	playerTrans_(_trans)
{
	//タグごとのヒット処理格納
	onHit_[Collider::TAG::PLAYER1] = [this](const std::weak_ptr<Collider> _hitCol) {};
	onHit_[Collider::TAG::PLAYER2] = [this](const std::weak_ptr<Collider> _hitCol) {};
	onHit_[Collider::TAG::PLAYER3] = [this](const std::weak_ptr<Collider> _hitCol) {};
	onHit_[Collider::TAG::PLAYER4] = [this](const std::weak_ptr<Collider> _hitCol) {};
	onHit_[Collider::TAG::NORMAL_OBJECT] = [this](const std::weak_ptr<Collider> _hitCol) {NormalObjectOnHit(_hitCol); };
	onHit_[Collider::TAG::TREE] = [this](const std::weak_ptr<Collider> _hitCol) {NormalObjectOnHit(_hitCol); };
	onHit_[Collider::TAG::GROUND] = [this](const std::weak_ptr<Collider> _hitCol) {GroundOnHit(_hitCol); };
	onHit_[Collider::TAG::MACHINE] = [this](const std::weak_ptr<Collider> _hitCol) {/*NormalObjectOnHit(_hitCol); */};
	onHit_[Collider::TAG::MACHINE_RIDE] = [this](const std::weak_ptr<Collider> _hitCol) {RideMachineOnHit(_hitCol); };
	onHit_[Collider::TAG::ITEM_BOX] = [this](const std::weak_ptr<Collider> _hitCol) {NormalObjectOnHit(_hitCol); };
	onHit_[Collider::TAG::POWER_UP] = [this](const std::weak_ptr<Collider> _hitCol) {PowerUpItemOnHit(_hitCol); };
	onHit_[Collider::TAG::DAMAGE] = [this](const std::weak_ptr<Collider> _hitCol) { SpinOnHit(_hitCol); };
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

void PlayerOnHit::OnHit(const std::weak_ptr<Collider> _hitCol)
{
	//タグ
	const auto& hitTag = _hitCol.lock()->GetTag();

	//タグごとのヒット処理
	onHit_[hitTag](_hitCol);
}

void PlayerOnHit::NormalObjectOnHit(const std::weak_ptr<Collider> _hitCol)
{
	//当たった形状情報
	const auto& hitGeo = _hitCol.lock()->GetGeometry();

	//コライダ
	auto& mainCol = player_.GetColliders()[static_cast<int>(Player::COL_VALUE::MAIN)];
	auto& groundPreCol = player_.GetColliders()[static_cast<int>(Player::COL_VALUE::GROUNDED_PRE)];
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

void PlayerOnHit::TreeOnHit(const std::weak_ptr<Collider> _hitCol)
{
	//各コライダ
	auto& mainCol = player_.GetColliders()[static_cast<int>(Player::COL_VALUE::MAIN)];
	auto& groundPreCol = player_.GetColliders()[static_cast<int>(Player::COL_VALUE::GROUNDED_PRE)];

	//相手モデル
	Model& model = dynamic_cast<Model&>(_hitCol.lock()->GetGeometry());
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

	for (int i = 0; i < hitNum; i++)
{
	auto hit = model.GetHitInfo().Dim[i];

	for (int tryCnt = 0; tryCnt < 10; tryCnt++)
	{
		int pHit = HitCheck_Sphere_Triangle(
			mainSphere.GetColPos(), radius,
			hit.Position[0], hit.Position[1], hit.Position[2]
		);

		if (pHit)
		{
			//当たった座標
			VECTOR hitPos = hit.HitPosition;

			//法線
			VECTOR normal = VNorm(hit.Normal);

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

			// 押し戻し
			if (maxDepth > 0.0f)
			{
				VECTOR N = VNorm(totalNormal);
				pos = VAdd(pos, VScale(N, maxDepth));
			}

			player_.SetMovedPos(pos);

			//playerTrans_.pos = player_.GetMovedPos();
			continue;
		}
		break;
	}
}

}

void PlayerOnHit::GroundOnHit(const std::weak_ptr<Collider> _hitCol)
{
	//各コライダ
	auto& mainCol = player_.GetColliders()[static_cast<int>(Player::COL_VALUE::MAIN)];
	auto& groundPreCol = player_.GetColliders()[static_cast<int>(Player::COL_VALUE::GROUNDED_PRE)];

	//相手モデル
	Model& model = dynamic_cast<Model&>(_hitCol.lock()->GetGeometry());
	const int hitNum = model.GetHitInfo().HitNum;

	//自身の線
	Line& line = dynamic_cast<Line&>(groundPreCol->GetGeometry());

	//自身の球
	Sphere& mainSphere = dynamic_cast<Sphere&>(mainCol->GetGeometry());
	float radius = mainSphere.GetRadius();

	//for (int i = 0; i < hitNum; i++)
	//{
	//	auto hit = model.GetHitInfo().Dim[i];

	//	for (int tryCnt = 0; tryCnt < 10; tryCnt++)
	//	{
	//		int pHit = HitCheck_Sphere_Triangle(
	//			mainSphere.GetColPos(), radius,
	//			hit.Position[0], hit.Position[1], hit.Position[2]
	//		);

	//		if (pHit)
	//		{
	//			player_.SetMovedPos(VAdd(player_.GetMovedPos(), VScale(hit.Normal, 3.0f)));

	//			playerTrans_.pos = player_.GetMovedPos();
	//			continue;
	//		}
	//		break;
	//	}
	//}

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
	//身体が当たっていないならスキップ
	if (!player_.GetColliders()[static_cast<int>(Player::COL_VALUE::MAIN)]->IsHit())return;

	//消失済みならスキップ
	if (_hitCol.lock()->IsDead())return;

	//SE
	SoundManager::GetInstance().Play(SoundManager::SOUND_NAME::GET_ITEM, SoundManager::PLAYTYPE::BACK);

	//対象パワーアップアイテム
	const auto& powerUpItem = dynamic_cast<const ItemBase&>(_hitCol.lock()->GetParent());

	//パラメーター
	Parameter param = powerUpItem.GetParam();

	//プレイヤーに加算
	player_.SetParam(player_.GetParam() + param);
}

void PlayerOnHit::SpinOnHit(const std::weak_ptr<Collider> _hitCol)
{
	//SE
	SoundManager::GetInstance().Play(SoundManager::SOUND_NAME::DAMAGE, SoundManager::PLAYTYPE::BACK);

	//スピンの相手
	const auto& spinParent = dynamic_cast<const Player&>(_hitCol.lock()->GetParent());

	//攻撃力
	float attack = spinParent.GetAttack();

	//ダメージ処理
	player_.Damage(attack);
}
