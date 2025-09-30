#pragma once
#include "ActionBase.h"
#include "../Machine/Machine.h"

class MachineAction : public ActionBase
{
public:

	//コンストラクタ
	MachineAction(Player& _player, const Machine _machine, const LogicBase& _logic);

	//コンストラクタ
	~MachineAction(void)override;

	//初期化
	void Init(void)override;

	//更新
	void Update(void)override;

	//描画
	void Draw(void)override;

private:

	//回転量の補正
	static constexpr float COMP_TURN = 1000.0f;

	//チャージ中の回転の補正値
	static constexpr float COMP_CHARGE_TURN = 10.0f;

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

	//旋回
	void Turn(void);
};

