#include"../pch.h"
#include "../Utility/Utility.h"
#include"../Manager/System/SceneManager.h"
#include"../Manager/System/Camera.h"
#include "../Manager/Game/MachineManager.h"
#include "../Manager/Game/GravityManager.h"
#include "../Common/Geometry/Sphere.h"
#include "../Common/Geometry/Line.h"
#include "Character/Character.h"
#include "Machine/Machine.h"
#include "Logic/LogicBase.h"
#include "Logic/UserLogic.h"
#include "Logic/NpcLogic.h"
#include "Action/ActionBase.h"
#include "Action/CharacterAction.h"
#include "Action/MachineAction.h"
#include "Collision/PlayerOnHit.h"
#include "Player.h"

Player::Player(std::weak_ptr<Camera> _camera, OPERATION_TYPE _operation, KeyConfig::JOYPAD_NO _padNo)
	:camera_(_camera)
{
	//初期化
	operation_ = _operation;
	padNo_ = _padNo;
	state_ = STATE::NONE;
	prePos_ = Utility::VECTOR_ZERO;
	movedPos_ = Utility::VECTOR_ZERO;
	movePow_ = Utility::VECTOR_ZERO;
	isGrounded_ = false;
	isSpin_ = false;
	footLine_ = Quaternion();
	damage_ = 0.0f;

	//操作タイプ
	createLogic_[OPERATION_TYPE::USER] = [this](void) {	logic_ = std::make_unique<UserLogic>(padNo_); };
	createLogic_[OPERATION_TYPE::NPC] = [this](void) {logic_ = std::make_unique<NpcLogic>(); };

	//行動切り替え
	changeAction_[STATE::NONE] = [this](void) {};
	changeAction_[STATE::NORMAL] = [this](void) {ChangeActionNormal(); };
	changeAction_[STATE::RIDE_MACHINE] = [this](void) {ChangeActionRide(); };

	//描画
	update_[STATE::NONE] = [this](void) {};
	update_[STATE::NORMAL] = [this](void) {UpdateNormal(); };
	update_[STATE::RIDE_MACHINE] = [this](void) {UpdateRide(); };

	//描画
	draw_[STATE::NONE] = [this](void) {};
	draw_[STATE::NORMAL] = [this](void) {DrawNormal(); };
	draw_[STATE::RIDE_MACHINE] = [this](void) {DrawRide(); };
}

Player::~Player(void)
{
}

void Player::Load(void)
{
}

void Player::Init(void)
{
	//キャラクター
	chara_ = std::make_unique<Character>();
	chara_->Load();
	chara_->Init();

	//初期機体情報
	const auto& machineMng = MachineManager::GetInstance();
	std::string name = "wakaba";
	int modelId = machineMng.GetInstance().GetModelId(name);
	float radius = machineMng.GetInstance().GetRadius(name);

	//機体
	machine_ = std::make_unique<Machine>(modelId, radius);
	machine_->Load();
	machine_->Init();

	//行動基準
	createLogic_[operation_]();

	//当たり判定後処理
	onHit_ = std::make_unique<PlayerOnHit>(*this, trans_);

	//初期状態
	ChangeState(STATE::RIDE_MACHINE);

	//当たり判定生成
	std::unique_ptr<Geometry> geo = std::make_unique<Sphere>(trans_.pos, movedPos_, NORMAL_RADIUS);
	MakeCollider(Collider::TAG::PLAYER1, std::move(geo), { Collider::TAG::PLAYER1,Collider::TAG::FOOT });

	//接地判定用の当たり判定
	geo = std::make_unique<Line>(trans_.pos, footLine_, LOCAL_LINE_UP, LOCAL_LINE_DOWN);
	MakeCollider(Collider::TAG::FOOT, std::move(geo), { Collider::TAG::PLAYER1,Collider::TAG::FOOT });

	//移動後接地判定
	//geo = std::make_unique<Line>(movedPos_, footLine_, LOCAL_LINE_UP, LOCAL_LINE_DOWN);
	//MakeCollider(Collider::TAG::PLAYER1, std::move(geo), { Collider::TAG::PLAYER1 });
	
	//体力
	damage_ = 0.0f;

	//初期更新
	Update();
}

void Player::Update(void)
{
	//インスタンス
	auto& grvMng = GravityManager::GetInstance();

	//移動更新
	prePos_ = trans_.pos;
	trans_.pos = movedPos_;

	//状態ごとの更新
	update_[state_]();

	//移動
	movedPos_ = VAdd(movedPos_, movePow_);

	//着地していないなら
	if (!isGrounded_)
	{
		//足元の回転をリセット
		footLine_ = Quaternion();
	}

	//接地判定初期化
	isGrounded_ = false;
}

void Player::Draw(void)
{
	//描画
	draw_[state_]();

	//for (auto& col : collider_)
	//{
	//	col->GetGeometry().Draw(0);
	//}

	//DrawFormatString(0, 32, 0xffffff, L"%.2f,%.2f,%.2f", trans_.quaRot.ToEuler().x, trans_.quaRot.ToEuler().y, trans_.quaRot.ToEuler().z);
	//DrawFormatString(0, 80, 0xffffff, L"%.2f,%.2f,%.2f", trans_.pos.x, trans_.pos.y, trans_.pos.z);
}

void Player::OnHit(const std::weak_ptr<Collider> _hitCol)
{
	//ヒット処理
	onHit_->OnHit(_hitCol);
}

void Player::SetParam(const Parameter& _param)
{
	//セット
	param_ = _param;

	//上下限値
	if (param_.maxSpeed_ > MAX_PARAM)param_.maxSpeed_ = MAX_PARAM;
	else if (param_.maxSpeed_ < 0)param_.maxSpeed_ = 0;

	if (param_.acceleration_ > MAX_PARAM)param_.acceleration_ = MAX_PARAM;
	else if (param_.acceleration_ < 0)param_.acceleration_ = 0;

	if (param_.turning_ > MAX_PARAM)param_.turning_ = MAX_PARAM;
	else if (param_.turning_ < 0)param_.turning_ = 0;

	if (param_.charge_ > MAX_PARAM)param_.charge_ = MAX_PARAM;
	else if (param_.charge_ < 0)param_.charge_ = 0;

	if (param_.flight_ > MAX_PARAM)param_.flight_ = MAX_PARAM;
	else if (param_.flight_ < 0)param_.flight_ = 0;

	if (param_.weight_ > MAX_PARAM)param_.weight_ = MAX_PARAM;
	else if (param_.weight_ < 0)param_.weight_ = 0;

	if (param_.attack_ > MAX_PARAM)param_.attack_ = MAX_PARAM;
	else if (param_.attack_ < 0)param_.attack_ = 0;

	if (param_.defence_ > MAX_PARAM)param_.defence_ = MAX_PARAM;
	else if (param_.defence_ < 0)param_.defence_ = 0;

	if (param_.maxHealth_ > MAX_PARAM)param_.maxHealth_ = MAX_PARAM;
	else if (param_.maxHealth_ < 0)param_.maxHealth_ = 0;
}

const Parameter Player::GetAllParam(void)const
{
	//総合パラメーター
	return param_.CalcUnitParam(GetUnitParam());
}

const float Player::GetAttack(void) const
{
	//パラメータ
	const Parameter& param = GetAllParam();

	return param.attack_ + (param.weight_ * WEIGHT_AFFECT) - (param.flight_ * FLIGHT_AFFECT);
}

const float Player::GetDefence(void) const
{
	//パラメータ
	const Parameter& param = GetAllParam();

	return param.defence_ + (param.weight_ * WEIGHT_AFFECT) - (param.flight_ * FLIGHT_AFFECT);
}

UnitParameter Player::GetUnitParam(void) const
{
	//キャラと機体の合計パラメーター
 	return chara_->GetUnitParam().Calculate(machine_->GetUnitParam());
}

void Player::ChangeState(const STATE& _state)
{
	//状態変更
	state_ = _state;

	//アクション状態遷移
	changeAction_[state_]();

	//初期化
	action_->Init();
}

void Player::SetIsSpin(const bool _isSpin)
{
	//同じなら何もしない
	if (isSpin_ == _isSpin)return;

	//判定
	isSpin_ = _isSpin;

	if (_isSpin)
	{
		//スピンコライダを生成
		std::unique_ptr<Geometry> geo = std::make_unique<Sphere>(trans_.pos, movedPos_, SPIN_RADIUS);
		MakeCollider(Collider::TAG::SPIN, std::move(geo), 
			{ Collider::TAG::PLAYER1,
			Collider::TAG::FOOT,
			Collider::TAG::NORMAL_OBJECT,
			Collider::TAG::GROUND,
			Collider::TAG::MACHINE_RIDE });
	}
	else
	{
		//スピンコライダを消す
		DeleteColliderAtTag(Collider::TAG::SPIN);
	}
}

void Player::RideMachine(std::unique_ptr<Machine> _machine)
{
	//コライダ削除
	_machine->DeleteCol();

	//機体格納
	machine_ = std::move(_machine);

	//機体乗車
	ChangeState(STATE::RIDE_MACHINE);
}

void Player::ChangeActionNormal(void)
{
	//キャラクターの行動に変更
	action_ = std::make_unique<CharacterAction>(*this, *chara_, *logic_);
	action_->Init();

	//カメラ状態の変更
	camera_.lock()->ChangeMode(Camera::MODE::FOLLOW);

	//アニメーションの変更
	chara_->GetAnim().Play("idle");
}

void Player::ChangeActionRide(void)
{
	//機体の行動に変更
	action_ = std::make_unique<MachineAction>(*this, *machine_, *logic_);
	action_->Init();

	//カメラ状態の変更
	camera_.lock()->ChangeMode(Camera::MODE::FOLLOW_LEAP);

	//アニメーションの変更
	chara_->GetAnim().Play("ride");
}

void Player::UpdateNormal(void)
{
	//行動
	action_->Update();

	//モデルと回転同期
	modelQuaRot_ = trans_.quaRot;

	//機体とキャラに座標と回転を同期させる
	SynchronizeChara();

	//アニメーション
	chara_->GetAnim().Update();
}

void Player::UpdateRide(void)
{
	//カメラ注視点までのベクトル
	VECTOR vec = Utility::GetMoveVec(trans_.pos, camera_.lock()->GetTargetPos());
	trans_.quaRot = trans_.quaRot.LookRotation(vec);

	//行動
	action_->Update();

	//スピン中か
	if (!isSpin_)
	{
		//モデルと回転同期
		modelQuaRot_ = trans_.quaRot;
	}
	else
	{
		//スピン
		modelQuaRot_ = modelQuaRot_.Mult(Quaternion::AngleAxis(Utility::Deg2RadF(SPIN_SPEED), Utility::AXIS_Y));
	}

	//降りたなら
	if (state_ == STATE::RIDE_MACHINE && logic_->IsGetOff())
	{
		//降りた機体を保存
		MachineManager::GetInstance().SaveGetOffMachine(std::move(machine_));

		//大きさを初期化
		trans_.scl = Utility::VECTOR_ONE;

		//キャラクターに遷移
		ChangeState(STATE::NORMAL);

		//処理終了
		return;
	}

	//機体とキャラに座標と回転を同期させる
	SynchronizeChara();
	SynchronizeMachine();
}

void Player::DrawNormal(void)
{
	//キャラの描画
	chara_->Draw();
}

void Player::DrawRide(void)
{
	//キャラの描画
	chara_->Draw();

	//機体の描画
	machine_->Draw();

	//UI
	action_->Draw();
}

void Player::SynchronizeChara(void)
{
	//座標と回転の同期
	chara_->SetPos(trans_.pos);
	chara_->SetScale(trans_.scl);
	chara_->SetQuaRot(modelQuaRot_);
	chara_->Update();
}

void Player::SynchronizeMachine(void)
{
	//座標と回転の同期
	machine_->SetPos(trans_.pos);
	//machine_->SetScale(trans_.scl);
	machine_->SetQuaRot(modelQuaRot_);
	machine_->Update();
}
