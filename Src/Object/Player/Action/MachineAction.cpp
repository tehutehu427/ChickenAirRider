#include "../Application.h"
#include "../Utility/Utility.h"
#include "../Manager/System/SceneManager.h"
#include "../Logic/LogicBase.h"
#include "MachineAction.h"

MachineAction::MachineAction(Player& _player, const Machine _machine, LogicBase& _logic)
	: ActionBase(_player,_logic),
	machine_(_machine)
{
	driveCnt_ = 0.0f;
	chargeCnt_ = 0.0f;
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
	//チャージをするか
	if (logic_.StartCharge())
	{
		//チャージ
		Charge();
	}
	else if(logic_.DisCharge())
	{
		//チャージ解放
		DisCharge();
	}
	else
	{
		//移動
		Move();
	}

	//回転
	Turn();
}

void MachineAction::Draw(void)
{
	//速度
	DrawFormatString(Application::SCREEN_SIZE_X - 320, Application::SCREEN_SIZE_Y - 32, 0xffffff, L"Speed = %.2f", speed_);
	DrawFormatString(Application::SCREEN_SIZE_X - 320, Application::SCREEN_SIZE_Y - 64, 0xffffff, L"Charge = %.2f", chargeCnt_);
}

void MachineAction::Move(void)
{
	//デルタタイム
	const auto& delta = SceneManager::GetInstance().GetDeltaTime();

	//パラメーター
	const auto& param = player_.GetAllParam();

	//加速力を元にだんだん最高速まで速度を増やす

	//カウンタ
	driveCnt_ += delta;

	//速度の方程式に当てはめる
	speed_ = velocity_ + (param.acceleration_ * delta);

	//最高速の制限
	if (speed_ > param.maxSpeed_ * MAX_SPEED_BASE)
	{
		//最高速
		//speed_ -= player_.GetAllParam().acceleration_ * driveCnt_;
		speed_ = param.maxSpeed_ * MAX_SPEED_BASE;
	}

	//前方に移動力を作る
	VECTOR movePow = VGet(0.0f, 0.0f, speed_);
	movePow = player_.GetTrans().quaRot.PosAxis(movePow);

	//プレイヤーに送る
	player_.SetMovePow(movePow);

	//チャージ力をだんだん減らす
	if (chargeCnt_ > 0.0f)
	{
		//chargeCnt_ -= player_.GetAllParam().acceleration_ * delta;
		//velocity_ = chargeCnt_;
		chargeCnt_ = 0.0f;
	}
}

void MachineAction::Charge(void)
{	
	//パラメーター
	const auto& param = player_.GetAllParam();
	const auto& unitParam = player_.GetUnitParam();

	//デルタタイム
	const auto& delta = SceneManager::GetInstance().GetDeltaTime();

	//チャージカウンタ
	chargeCnt_ += delta * player_.GetAllParam().charge_;

	//チャージの制限
	if (chargeCnt_ > unitParam.chargeCapacity_)
	{
		chargeCnt_ = param.maxSpeed_ * 7;
	}

	//ターンしていない
	if (logic_.TurnValue() == 0.0f)
	{
		//減速
		//speed_ -= SceneManager::GetInstance().GetDeltaTime() * player_.GetAllParam().acceleration_ * 100.0f;
		speed_ = 0.0f;

		//速度の下限
		if (speed_ < 0.0f)
		{
			speed_ = 0.0f;
		}

		//前方に移動力を作る
		VECTOR movePow = VGet(0.0f, 0.0f, speed_);
		movePow = player_.GetTrans().quaRot.PosAxis(movePow);

		//プレイヤーに送る
		player_.SetMovePow(movePow);
	}
}

void MachineAction::DisCharge(void)
{
	//走行時間初期化
	driveCnt_ = 0.0f;

	//チャージ分を初速度に変換
	velocity_ = chargeCnt_;

	//初速度
	//speed_ = velocity_;
}

void MachineAction::Turn(void)
{
	//パラメーター
	const auto& param = player_.GetAllParam();

	//回転量
	float turnPow = logic_.TurnValue() / COMP_TURN;

	//回転量がないならスキップ
	if (turnPow == 0.0f)return;

	//パラメーターで回転しやすくする
	turnPow += turnPow > 0.0f ? param.turning_ / COMP_TURN
		: -param.turning_ / COMP_TURN;

	//回転
	player_.SetQuaRot(player_.GetTrans().quaRot.Mult(Quaternion::AngleAxis(Utility::Deg2RadF(turnPow), Utility::AXIS_Y)));
}
