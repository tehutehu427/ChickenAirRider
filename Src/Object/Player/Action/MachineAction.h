#pragma once
#include "ActionBase.h"
#include "../Machine/Machine.h"

class MachineAction : public ActionBase
{
public:

	//コンストラクタ
	MachineAction(Player& _player, const Machine _machine, LogicBase& _logic);

	//コンストラクタ
	~MachineAction(void)override;

	//初期化
	void Init(void)override;

	//更新
	void Update(void)override;

	//描画
	void Draw(void)override;

private:

	//最高速の基本倍率
	static constexpr int MAX_SPEED_BASE = 10;

	//回転量の補正
	static constexpr float COMP_TURN = 500.0f;

	//機体
	const Machine& machine_;

	//走行時間カウンタ
	float driveCnt_;

	//チャージカウンタ
	float chargeCnt_;

	//初速度
	float velocity_;

	//総合速度
	float speed_;

	//移動
	void Move(void);

	//チャージ
	void Charge(void);

	//チャージ解放
	void DisCharge(void);

	//旋回
	void Turn(void);
};

