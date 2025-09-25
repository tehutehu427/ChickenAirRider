#pragma once
#include "ActionBase.h"

class MachineAction : public ActionBase
{
public:

	//コンストラクタ
	MachineAction(Player& _player, const LogicBase& _logic);

	//コンストラクタ
	~MachineAction(void)override;

	//初期化
	void Init(void)override;

	//更新
	void Update(void)override;

	//描画
	void Draw(void)override;

private:

	//移動
	void Move(void);

	//チャージ
	void Charge(void);

	//旋回
	void Turn(void);
};

