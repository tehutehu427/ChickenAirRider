#pragma once
#include <map>
#include <functional>
#include "ActionBase.h"
#include "../Machine/Machine.h"

class MachineAction : public ActionBase
{
public:

	/// @brief コンストラクタ
	/// @param _player 親情報
	/// @param _machine 機体の情報
	/// @param _logic 行動情報
	MachineAction(Player& _player, const Machine _machine, LogicBase& _logic);

	//デストラクタ
	~MachineAction(void)override;

	//初期化
	void Init(void)override;

	//更新
	void Update(void)override;

	//描画
	void Draw(void)override;

private:
	
	//最高速の基本倍率
	static constexpr float BASE_MAX_SPEED = 10.0f;

	//加速度の固定上昇値
	static constexpr float BASE_ACCELE = 5.0f;

	//チャージの固定上昇値
	static constexpr float BASE_CHARGE = 50.0f;

	//回転量の補正
	static constexpr float COMP_TURN = 500.0f;

	//機体
	const Machine& machine_;

	//移動力
	VECTOR movePow_;

	//走行時間カウンタ
	float driveCnt_;

	//チャージカウンタ
	float chargeCnt_;

	//初速度
	float velocity_;

	//総合速度
	float speed_;

	//飛行力
	float flightPow_;

	//地面から離れた瞬間
	bool isFlightNow_;

	//重力
	VECTOR gravPow_;

	//状態ごとの更新
	std::map<bool, std::function<void(void)>> update_;

	//状態ごとの更新
	void UpdateGround(void);
	void UpdateFlight(void);

	//移動
	void Move(void);

	//チャージ
	void Charge(void);

	//チャージ解放
	void DisCharge(void);

	//旋回
	void Turn(void);

	//飛行
	void Flight(void);

	//飛行の移動
	void FlightMove(void);
};

