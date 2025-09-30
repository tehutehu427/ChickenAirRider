#include "../Application.h"
#include "../Utility/Utility.h"
#include "../Manager/System/SceneManager.h"
#include "../Logic/LogicBase.h"
#include "MachineAction.h"

MachineAction::MachineAction(Player& _player, const Machine _machine, const LogicBase& _logic)
	: ActionBase(_player,_logic),
	machine_(_machine)
{
	driveCnt_ = 0.0f;
	velocity_ = 0.0f;
	speed_ = 0.0f;
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
	//移動
	Move();

	//回転
	Turn();
}

void MachineAction::Draw(void)
{
	//速度
	DrawFormatString(Application::SCREEN_SIZE_X - 320, Application::SCREEN_SIZE_Y - 32, 0xffffff, L"Speed = %.2f", speed_);
}

void MachineAction::Move(void)
{
	//加速力を元にだんだん最高速まで速度を増やす

	//カウンタ
	driveCnt_ += SceneManager::GetInstance().GetDeltaTime();

	//初速度
	const float VELOCITY = 3.0f;

	//速度の方程式に当てはめる
	speed_ = VELOCITY + (player_.GetAllParam().acceleration_ * driveCnt_);

	//最高速の制限
	if (speed_ > player_.GetAllParam().maxSpeed_ * 10)
	{
		//最高速
		speed_ = static_cast<float>(player_.GetAllParam().maxSpeed_ * 10);
	}

	//前方に移動力を作る
	VECTOR movePow = VGet(0.0f, 0.0f, speed_);
	movePow = player_.GetTrans().quaRot.PosAxis(movePow);

	//プレイヤーに送る
	player_.SetMovePow(movePow);
}

void MachineAction::Charge(void)
{
}

void MachineAction::Turn(void)
{
	//回転量
	float rotatePow = logic_.TurnValue() / COMP_ROTATE;

	//パラメーターで回転しやすくする
	rotatePow += rotatePow >= 0.0f ? player_.GetAllParam().turning_ / COMP_ROTATE : -player_.GetAllParam().turning_ / COMP_ROTATE;

	//回転
	player_.SetQuaRot(player_.GetTrans().quaRot.Mult(Quaternion::AngleAxis(Utility::Deg2RadF(rotatePow), Utility::AXIS_Y)));
}
