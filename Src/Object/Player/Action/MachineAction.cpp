#include "../Manager/System/SceneManager.h"
#include "MachineAction.h"

MachineAction::MachineAction(Player& _player, const Machine _machine, const LogicBase& _logic)
	: ActionBase(_player,_logic),
	machine_(_machine)
{
	driveCnt_ = 0.0f;
	velocity_ = 0.0f;
}

MachineAction::~MachineAction(void)
{
}

void MachineAction::Init(void)
{
	//最小初速度等は外部から
}

void MachineAction::Update(void)
{
}

void MachineAction::Draw(void)
{
}

void MachineAction::Move(void)
{
	//加速力を元にだんだん最高速まで速度を増やす

	//カウンタ
	driveCnt_ += SceneManager::GetInstance().GetDeltaTime();

	//初速度
	const float VELOCITY = 3.0f;

	//速度の方程式に当てはめる
	float speed = VELOCITY + (player_.GetAllParam().acceleration_ * driveCnt_);

	//最高速の制限
	if (speed > player_.GetAllParam().maxSpeed_ * 10)
	{
		//最高速
		speed = static_cast<float>(player_.GetAllParam().maxSpeed_ * 10);
	}

	//前方に移動力を作る
	VECTOR movePow = VGet(0.0f, 0.0f, speed);
	movePow = player_.GetTrans().quaRot.PosAxis(movePow);

	//プレイヤーに送る
	player_.SetMovePow(movePow);
}

void MachineAction::Charge(void)
{
}

void MachineAction::Turn(void)
{
}
