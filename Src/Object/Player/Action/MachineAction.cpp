#include"../pch.h"
#include "../Application.h"
#include "../Utility/Utility.h"
#include "../Manager/System/SceneManager.h"
#include "../Manager/System/Camera.h"
#include "../Manager/Game/GravityManager.h"
#include "../Manager/Game/UIManager.h"
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
	spinCnt_ = 0.0f;

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
	//デルタタイム
	const auto& delta = SceneManager::GetInstance().GetDeltaTime();

	//スピン判定
	if (logic_.IsButtonMeshing() && !player_.IsSpin())
	{
		//スピン開始
		player_.SetIsSpin(true);
	}

	//スピン時間
	if (player_.IsSpin())
	{
		//カウンタ
		spinCnt_ += delta;

		//スピン時間
		if (spinCnt_ > SPIN_TIME)
		{
			//初期化
			spinCnt_ = 0.0f;

			//スピン終了
			player_.SetIsSpin(false);
		}
	}

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
	const SceneManager& scnMng = SceneManager::GetInstance();
	const UIManager& uiMng = UIManager::GetInstance();
	const int index = player_.GetPlayerIndex();
	const auto& viewPort = uiMng.GetViewPort(index);

	const int playerIndex = player_.GetPlayerIndex();
	const Parameter& param = player_.GetAllParam();
	const float nowHealth = player_.GetNowHealth();

	int posX = viewPort.x;
	int posY = viewPort.y;

	//速度
	DrawFormatString(posX - 320, posY - 32, 0xffffff, L"Speed = %.2f", speed_);
	//DrawFormatString(Application::SCREEN_SIZE_X - 320, Application::SCREEN_SIZE_Y - 64, 0xffffff, L"Charge = %.2f", chargeCnt_);

	const int div = 64; // 分割数（多いほど滑らか）
	const float maxAngle = chargeCnt_ * DX_TWO_PI;


	int chargePosX = posX + static_cast<int>(CHARGE_POS.x * viewPort.w);
	int chargePosY = posY + static_cast<int>(CHARGE_POS.y * viewPort.h);
	float cx = chargePosX;
	float cy = chargePosY;
	float radius = 25.0f;
	float prevX = cx;
	float prevY = cy - radius; // 角度0（上）から開始したい場合

	for (int i = 0; i <= div; i++)
	{
		float t = (float)i / div;
		float angle = -DX_PI / 2.0f + maxAngle * t; // 上方向から時計回り

		float x = cx + cosf(angle) * radius;
		float y = cy + sinf(angle) * radius;

		// 中心 + 直前の点 + 今の点 の三角形を描画
		DrawTriangle(
			cx, cy,
			prevX, prevY,
			x, y,
			Utility::BLUE, TRUE
		);

		prevX = x;
		prevY = y;
		if (t >= chargeCnt_) break; // 余計な描画を防ぐ
	}

	int healthPosX = posX + static_cast<int>(HEALTH_POS.x * viewPort.w);
	int healthPosY = posY + static_cast<int>(HEALTH_POS.y * viewPort.h);

	//最大体力
	DrawBox(healthPosX - HEALTH_BOX_LOCAL_POS_X,
		healthPosY + HEALTH_BOX_LOCAL_POS_Y,
		healthPosX + HEALTH_BOX_LOCAL_POS_X,
		healthPosY + HEALTH_BOX_LOCAL_POS_Y - param.maxHealth_ * HEALTH_BOX,
		Utility::BLACK, true);
	
	if (nowHealth > 0.0f)
	{
		//体力
		DrawBox(healthPosX - HEALTH_BOX_LOCAL_POS_X + HEALTH_LOCAL,
			healthPosY + HEALTH_BOX_LOCAL_POS_Y - HEALTH_LOCAL,
			healthPosX + HEALTH_BOX_LOCAL_POS_X - HEALTH_LOCAL,
			healthPosY + HEALTH_BOX_LOCAL_POS_Y - (param.maxHealth_ * HEALTH_BOX) * (nowHealth / param.maxHealth_) + HEALTH_LOCAL,
			Utility::RED, true);
	}
}

void MachineAction::UpdateGround(void)
{
	//飛んだ判定のリセット
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
	
	//チャージを解放したか
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
	if (speed_ > param.maxSpeed_ * BASE_MAX_SPEED)
	{
		//減速なのでカウンタ減少
		driveCnt_ -= delta;
	}
	else if (!player_.IsGrounded())
	{
		//減速なのでカウンタ減少
		driveCnt_ -= delta / FLIGHT_DECELERATION;
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
	else
	{
		VECTOR scale = player_.GetTrans().scl;
		scale.y -= chargeCnt_ / CHARGE_MODEL_TRANS;
		scale.x += chargeCnt_ / CHARGE_MODEL_TRANS;
		scale.z += chargeCnt_ / CHARGE_MODEL_TRANS;
		player_.SetScale(scale);
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
		//チャージがたまるまで
		if (chargeCnt_ < 1.0f)
		{
			//減速は少なく
			driveCnt_ -= delta;
		}
		else
		{
			//減速
			driveCnt_ -= delta * TURN_BRAKE_POW;
		}
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

	//大きさ初期化
	player_.SetScale(Utility::VECTOR_ONE);
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

	//チャージ中だと回転しやすい
	if(player_.IsGrounded())turnPow = logic_.StartCharge() ? turnPow * CHARGE_TURN : turnPow;

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
	axis_.x = Utility::Deg2RadF(std::clamp(Utility::Rad2DegF(axis_.x), -AXIS_X_LIMIT, AXIS_X_LIMIT));

	//重力制御
	GravityManager::GetInstance().CalcGravity(Utility::DIR_D, gravPow_);

	//だんだん落ちていく
	flightPow_ += (delta * gravPow_.y * param.weight_ / BASE_WEIGHT) / param.flight_ * BASE_FLIGHT;
}