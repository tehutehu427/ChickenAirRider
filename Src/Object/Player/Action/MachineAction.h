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

	//機体
	const Machine& machine_;

	//走行時間カウンタ
	float driveCnt_;

	//初速度
	float velocity_;

	//移動
	void Move(void);

	//チャージ
	void Charge(void);

	//旋回
	void Turn(void);
};

