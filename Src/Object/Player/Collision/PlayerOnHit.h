#pragma once
#include<unordered_map>
#include<functional>
#include"../Player.h"
#include"../../Common/Collider.h"

class PlayerOnHit
{
public:

	//コンストラクタ
	PlayerOnHit(Player& _player);

	//デストラクタ
	~PlayerOnHit(void);

	//データのロード
	void Load(void);

	//ヒット処理
	void OnHit(const Collider* _hitCol);

private:

	//押し戻しの補正値
	static constexpr float FOOT_COMP = 5.0f;

	//無敵時間
	static constexpr float INVINCIBLE_SPIN = 0.2f;

	//親
	Player& player_;

	//コライダごとのヒット処理
	using HitFunc = void(PlayerOnHit::*)(const Collider* _hitCol);
	std::array<HitFunc, static_cast<int>(Collider::TAG::MAX)> onHit_;

	//ヒット処理
	void NormalObjectOnHit(const const Collider* _hitCol);
	void GroundOnHit(const Collider* _hitCol);
	void RideMachineOnHit(const Collider* _hitCol);
	void PowerUpItemOnHit(const Collider* _hitCol);
	void BattleItemOnHit(const Collider* _hitCol);
	void SpinOnHit(const Collider* _hitCol);
	void CannonShotOnHit(const Collider* _hitCol);
	void GlideStageOnHit(const Collider* _hitCol);
};

