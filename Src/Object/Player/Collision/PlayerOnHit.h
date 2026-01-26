#pragma once
#include<unordered_map>
#include<functional>
#include"../Player.h"
#include"../../Common/Collider.h"

class PlayerOnHit
{
public:

	/// @brief コンストラクタ
	/// @param _player 親
	PlayerOnHit(Player& _player, Transform& _trans);

	//デストラクタ
	~PlayerOnHit(void);

	//データのロード
	void Load(void);

	//ヒット処理
	void OnHit(const std::weak_ptr<Collider> _hitCol);

private:

	//押し戻しの補正値
	static constexpr float FOOT_COMP = 5.0f;

	//無敵時間
	static constexpr float INVINCIBLE_SPIN = 0.2f;

	//親
	Player& player_;
	Transform& playerTrans_;

	//コライダごとのヒット処理
	std::unordered_map<Collider::TAG, std::function<void(const std::weak_ptr<Collider> _hitCol)>> onHit_;

	//ヒット処理
	void NormalObjectOnHit(const std::weak_ptr<Collider> _hitCol);
	void GroundOnHit(const std::weak_ptr<Collider> _hitCol);
	void RideMachineOnHit(const std::weak_ptr<Collider> _hitCol);
	void PowerUpItemOnHit(const std::weak_ptr<Collider> _hitCol);
	void BattleItemOnHit(const std::weak_ptr<Collider> _hitCol);
	void SpinOnHit(const std::weak_ptr<Collider> _hitCol);
	void CannonShotOnHit(const std::weak_ptr<Collider> _hitCol);
	void GlideStageOnHit(const std::weak_ptr<Collider> _hitCol);
};

