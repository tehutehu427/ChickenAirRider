#include"../pch.h"
#include "../Application.h"
#include "../Utility/Utility.h"
#include "../Manager/System/SceneManager.h"
#include "../Manager/System/Camera.h"
#include "../Manager/Game/GravityManager.h"
#include "../Logic/LogicBase.h"
#include "MachineAction.h"

MachineAction::MachineAction(Player& _player, const Machine& _machine, LogicBase& _logic)
	: ActionBase(_player,_logic),
	machine_(_machine)
{
	driveCnt_ = 0.0f;
	chargeCnt_ = 0.0f;
	velocity_ = 0.0f;
	speed_ = 0.0f;
	flightPow_ = 0.0f;
	isFlightNow_ = false;
	gravPow_ = Utility::VECTOR_ZERO;

	update_[true] = [this](void) {UpdateGround(); };
	update_[false] = [this](void) {UpdateFlight(); };
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
	//回転更新
	player_.SetQuaRot(player_.GetTrans().quaRot.Mult(Quaternion::Euler(axis_)));

	//状態ごとの更新
	update_[player_.IsGrounded()]();

	//最終的な移動力の合算
	movePow_ = VGet(0.0f, flightPow_, speed_);
	movePow_ = player_.GetTrans().quaRot.PosAxis(movePow_);

	//プレイヤーに送る
	player_.SetMovePow(movePow_);
}

void MachineAction::Draw(void)
{
	//速度
	DrawFormatString(Application::SCREEN_SIZE_X - 320, Application::SCREEN_SIZE_Y - 32, 0xffffff, L"Speed = %.2f", speed_);
	DrawFormatString(Application::SCREEN_SIZE_X - 320, Application::SCREEN_SIZE_Y - 64, 0xffffff, L"Charge = %.2f", chargeCnt_);
}

void MachineAction::UpdateGround(void)
{
	if (isFlightNow_)
	{
		isFlightNow_ = false;
	}

	//チャージをするか
	if (logic_.StartCharge())
	{
		//チャージ
		Charge();
	}
	
	if (logic_.DisCharge())
	{
		//チャージ解放
		DisCharge();
	}

	//移動
	Move();

	//回転
	Turn();
}

void MachineAction::UpdateFlight(void)
{
	//移動
	Move();

	//飛行
	Flight();

	//回転
	Turn();
}

void MachineAction::Move(void)
{
	//デルタタイム
	const auto& delta = SceneManager::GetInstance().GetDeltaTime();

	//パラメーター
	const auto& param = player_.GetAllParam();

	//加速力を元にだんだん最高速まで速度を増やす

	//チャージ中　かつ　地上にいないなら加速しない
	if (!logic_.StartCharge() && player_.IsGrounded())
	{
		if(speed_ < param.maxSpeed_ * BASE_MAX_SPEED)
		{
			//カウンタ
			driveCnt_ += delta;
		}
	}

	//最高速の制限　又は　飛行中なら
	if (speed_ > param.maxSpeed_ * BASE_MAX_SPEED || !player_.IsGrounded())
	{
		//減速なのでカウンタ減少
		driveCnt_ -= delta;
	}

	//速度の方程式に当てはめる
	speed_ = velocity_ + (param.acceleration_ * driveCnt_);

	//速度が負の値にならないようにする
	if (speed_ < 0.0f)
	{
		speed_ = 0.0f;
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
	chargeCnt_ += BASE_CHARGE * param.charge_ / unitParam.chargeCapacity_;

	//チャージの制限
	if (chargeCnt_ > 1.0f)
	{
		chargeCnt_ = 1.0f;
	}

	//速度の下限
	if (speed_ < 0.0f)return;

	//ターンしていない
	if (logic_.TurnValue().x == 0.0f)
	{
		//減速
		driveCnt_ -= delta * BRAKE_POW;
	}
	else
	{
		//減速
		driveCnt_ -= delta * TURN_BRAKE_POW;
	}
}

void MachineAction::DisCharge(void)
{
	//パラメーター
	const auto& param = player_.GetAllParam();
	const auto& unitParam = player_.GetUnitParam();

	//チャージの割合で初速度を決める
	float velocity = (param.maxSpeed_ * BASE_MAX_SPEED) * (1 + unitParam.boostRate_ * std::pow(chargeCnt_,unitParam.boostPower_)) * std::pow(chargeCnt_,unitParam.chargeDamp_);

	//速度
	if (speed_ < velocity)
	{
		//走行時間の初期化
		driveCnt_ = 0.0f;

		velocity_ = velocity;
	}

	//チャージを初期化
	chargeCnt_ = 0.0f;
}

void MachineAction::Turn(void)
{
	//パラメーター
	const auto& param = player_.GetAllParam();
	
	//デルタタイム
	const auto& delta = SceneManager::GetInstance().GetDeltaTime();

	//回転量
	float turnPow = logic_.TurnValue().x * delta;

	//回転量がないならスキップ
	if (turnPow == 0.0f)return;

	//パラメーターで回転しやすくする
	turnPow += turnPow > 0.0f ? param.turning_ * delta
		: -param.turning_ * delta;

	if(player_.IsGrounded())turnPow = logic_.StartCharge() ? turnPow * 2.0f : turnPow;

	//Y回転を設定
	player_.GetCamera().lock()->SetRotPow(std::fabsf(Utility::Deg2RadF(turnPow)));
}

void MachineAction::Flight(void)
{
	//パラメーター
	const auto& param = player_.GetAllParam();

	//デルタタイム
	const auto& delta = SceneManager::GetInstance().GetDeltaTime();

	//飛んだ瞬間
	if (!isFlightNow_)
	{
		//移動力を保持
		flightPow_ = speed_ * delta;

		//飛び立った判定
		isFlightNow_ = true;
	}

	//回転量
	float flightPow = logic_.TurnValue().y * delta;

	//X回転を設定
	axis_.x += flightPow;
	axis_.x = Utility::Deg2RadF(std::clamp(Utility::Rad2DegF(axis_.x), -45.0f, 45.0f));

	//重力制御
	GravityManager::GetInstance().CalcGravity(Utility::DIR_D, gravPow_);

	//だんだん落ちていく
	flightPow_ += delta * (gravPow_.y) + (flightPow * param.flight_);
}