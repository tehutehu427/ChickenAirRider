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
	PlayerOnHit(Player& _player);

	//デストラクタ
	~PlayerOnHit(void);

	//ヒット処理
	void OnHit(const std::weak_ptr<Collider> _hitCol);

private:

	//親
	Player& player_;

	//コライダごとのヒット処理
	std::unordered_map<Collider::TAG, std::function<void(const std::weak_ptr<Collider> _hitCol)>> onHit_;

	//ヒット処理
	void NormalObjectOnHit(const std::weak_ptr<Collider> _hitCol);
	void RideMachineOnHit(const std::weak_ptr<Collider> _hitCol);
};

