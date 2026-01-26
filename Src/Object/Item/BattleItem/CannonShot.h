#pragma once
#include<functional>
#include<unordered_map>
#include "../../ObjectBase.h"

class Player;

class CannonShot : public ObjectBase
{
public:

	//無敵時間
	static constexpr float INVINCIBLE = 0.5f;

	//コンストラクタ
	CannonShot(const VECTOR& _pos, const Quaternion& _rot, const VECTOR& _scl, const std::weak_ptr<Collider> _holder, const float _speed);

	//デストラクタ
	~CannonShot(void)override;

	//読み込み
	void Load(void)override;

	//初期化
	void Init(void)override;

	//更新
	void Update(void)override;

	//描画
	void Draw(void)override;

	//当たり判定
	void OnHit(const std::weak_ptr<Collider> _hitCol)override;

	//攻撃力を返す
	const float GetAttack(void)const { return attack_; }

	//生死状態
	const bool IsAlive(void) { return state_ != STATE::DEAD; }

private:

	//状態
	enum class STATE
	{
		ALIVE,	//生存
		BLAST,	//爆発
		DEAD,	//死亡
	};

	enum class COL
	{
		MAIN,	//メイン
		SEARCH,	//索敵
	};

	//速度
	static constexpr float SPEED = 50.0f;

	//半径
	static constexpr float SHOT_RADIUS = 50.0f;
	static constexpr float BLAST_RADIUS = 150.0f;
	static constexpr float SEARCH_RADIUS = 600.0f;
	static constexpr float BROUD_RADIUS = 650.0f;

	//生存時間
	static constexpr float ALIVE_TIME = 10.0f;
	static constexpr float BLAST_TIME = 0.3f;

	//攻撃力補正
	static constexpr float ATTACK_MULTI = 2.0f;

	//索敵補正値
	static constexpr float SEARCH_MOVE_POW_MULTI = 1.0f;

	//所有者タグ
	std::weak_ptr<Collider> holder_;

	//移動後座標
	VECTOR movedPos_;
	
	//重力
	VECTOR gravPow_;

	//移動力
	VECTOR movePow_;
	VECTOR movePowToTarget_;

	//カウンタ
	float aliveCnt_;
	float blastCnt_;

	//状態
	STATE state_;

	//攻撃力
	float attack_;

	//速度
	float speed_;

	//更新
	std::unordered_map<STATE, std::function<void(void)>> update_;

	//描画
	std::unordered_map<STATE, std::function<void(void)>> draw_;

	//各更新
	void UpdateAlive(void);
	void UpdateBlast(void);
	void UpdateDead(void);

	//各描画
	void DrawAlive(void);
	void DrawBlast(void);
	void DrawDead(void);
};

