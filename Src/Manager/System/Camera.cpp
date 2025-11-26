#include"../pch.h"
#include "../../Utility/Utility.h"
#include "../../Object/Common/Transform.h"
#include "../../Manager/System/SceneManager.h"
#include "../../Manager/Game/GravityManager.h"
#include "../../Application.h"
#include "Camera.h"

Camera::Camera(int _userNum)
{
	angles_ = VECTOR();
	cameraUp_ = VECTOR();
	mode_ = MODE::NONE;
	pos_ = Utility::VECTOR_ZERO;
	targetPos_ = Utility::VECTOR_ZERO;
	followTransform_ = nullptr;
	padNo_ = static_cast<KeyConfig::JOYPAD_NO>(_userNum + 1);
	localPos_ = Utility::VECTOR_ZERO;
	rotPow_ = 0.0f;
	leapTargetPos_ = Utility::VECTOR_ZERO;
}

Camera::~Camera(void)
{
}

void Camera::Init(void)
{

	ChangeMode(MODE::FIXED_POINT);

}

void Camera::Update(void)
{
}

void Camera::SetBeforeDraw(void)
{

	// クリップ距離を設定する(SetDrawScreenでリセットされる)
	SetCameraNearFar(CAMERA_NEAR, CAMERA_FAR);

	switch (mode_)
	{
	case Camera::MODE::FIXED_POINT:
		SetBeforeDrawFixedPoint();
		break;
	case Camera::MODE::FOLLOW:
		SetBeforeDrawFollow();
		break;
	case Camera::MODE::FOLLOW_LEAP:
		SetBeforeDrawFollowLeap();
		break;
	case Camera::MODE::FOLLOW_ROTATION:
		SetBeforeDrawFollowRotation();
		break;
	case Camera::MODE::SELF_SHOT:
		SetBeforeDrawSelfShot();
		break;
	case Camera::MODE::FPS:
		SetBeforeDrawFPS();
		break;
	case Camera::MODE::FREE_CONTROLL:
		SetBeforeDrawFreeControll();
		break;
	case Camera::MODE::FIXED_UP:
		SetBeforeDrawFixedUp();
		break;
	case Camera::MODE::FIXED_DIAGONAL:
		SetBeforeDrawFixedDiagonal();
		break;
	}

	//カメラの設定
	CameraSetting();

	// DXライブラリのカメラとEffekseerのカメラを同期する。
	Effekseer_Sync3DSetting();

}

void Camera::CameraSetting()
{
	// カメラの設定(位置と注視点による制御)
	SetCameraPositionAndTargetAndUpVec(
		pos_,
		targetPos_,
		cameraUp_
	);
}

void Camera::Draw(void)
{
}

void Camera::SetFollow(const Transform* follow)
{
	followTransform_ = follow;
}

VECTOR Camera::GetPos(void) const
{
	return pos_;
}

VECTOR Camera::GetAngles(void) const
{
	return angles_;
}

VECTOR Camera::GetTargetPos(void) const
{
	return targetPos_;
}

Quaternion Camera::GetQuaRot(void) const
{
	return rot_;
}

Quaternion Camera::GetQuaRotOutX(void) const
{
	return rotOutX_;
}

VECTOR Camera::GetForward(void) const
{
	return VNorm(VSub(targetPos_, pos_));
}

void Camera::ChangeMode(MODE mode)
{

	// カメラの初期設定
	SetDefault();

	// カメラモードの変更
	mode_ = mode;

	// 変更時の初期化処理
	switch (mode_)
	{
	case Camera::MODE::FIXED_POINT:
		break;
	case Camera::MODE::FOLLOW:
		localPos_ = LOCAL_F2C_POS;
		break;
	case Camera::MODE::FOLLOW_LEAP:
		localPos_ = LOCAL_F2C_LEAP_POS;
		break;
	}

}

void Camera::SetDefault(void)
{

	// カメラの初期設定
	pos_ = DEFAULT_CAMERA_POS;

	// 注視点
	targetPos_ = Utility::VECTOR_ZERO;

	// カメラの上方向
	cameraUp_ = Utility::DIR_U;

	angles_.x = Utility::Deg2RadF(30.0f);
	angles_.y = 0.0f;
	angles_.z = 0.0f;
	rot_ = Quaternion();

}

void Camera::SyncFollow(void)
{

	// 同期先の位置
	VECTOR pos = followTransform_->pos;

	// 重力の方向制御に従う
	Quaternion gRot = Quaternion::Euler(VECTOR(0.0, 0.0, 0.0));

	// 正面から設定されたY軸分、回転させる
	rotOutX_ = gRot.Mult(Quaternion::AngleAxis(angles_.y, Utility::AXIS_Y));

	// 正面から設定されたX軸分、回転させる
	rot_ = rotOutX_.Mult(Quaternion::AngleAxis(angles_.x, Utility::AXIS_X));

	VECTOR localPos;

	// 注視点(通常重力でいうところのY値を追従対象と同じにする)
	localPos = rotOutX_.PosAxis(LOCAL_F2T_POS);
	targetPos_ = VAdd(pos, localPos);

	// カメラ位置
	localPos = rot_.PosAxis(localPos_);

	//補間
	pos_ = VAdd(pos, localPos);

	// カメラの上方向
	cameraUp_ = gRot.GetUp();
}

void Camera::SyncFollowLeap(void)
{
	// --- プレイヤーの位置 ---
	VECTOR pos = followTransform_->pos;

	// --- プレイヤーの速度（前フレームとの差から算出） ---
	static VECTOR oldPos = pos;
	VECTOR velocity = VSub(pos, oldPos);
	oldPos = pos;

	// --- 重力方向制御（今は固定） ---
	Quaternion gRot = Quaternion::Euler(VECTOR(0.0, 0.0, 0.0));

	// --- 回転設定 ---
	rotOutX_ = gRot.Mult(Quaternion::AngleAxis(angles_.y, Utility::AXIS_Y));
	rot_ = rotOutX_;

	// --- カメラ方向ベクトル ---
	VECTOR forward = rot_.PosAxis(VGet(0, 0, 1));
	VECTOR up = rot_.PosAxis(VGet(0, 1, 0));

	// --- プレイヤーの移動方向成分を前方向に投影 ---
	float speedFront = VDot(velocity, forward);

	// --- 注視点 ---
	VECTOR localPos = rotOutX_.PosAxis(LOCAL_F2T_LEAP_POS);
	VECTOR desiredTargetPos = VAdd(pos, localPos);

	// --- 移動速度に応じて注視点を前に押し出す ---
	float lookAhead = std::clamp(speedFront * 15.0f, -100.0f, 300.0f);
	desiredTargetPos = VAdd(desiredTargetPos, VScale(forward, lookAhead));

	// --- カメラ理想位置 ---
	localPos = rot_.PosAxis(localPos_);
	VECTOR desiredCameraPos = VAdd(pos, localPos);

	// --- 差分 ---
	VECTOR diff = VSub(desiredCameraPos, pos_);

	// forward方向成分
	float front = VDot(diff, forward);
	VECTOR frontMove = VScale(forward, front);

	// up方向成分
	float upAmount = VDot(diff, up);
	VECTOR upMove = VScale(up, upAmount);

	// side方向成分
	VECTOR temp = VAdd(frontMove, upMove);
	VECTOR sideMove = VSub(diff, temp);

	// --- 補間係数 ---
	const float rateFront = 0.9f;	// 前後：速く追従
	const float rateSide = 0.05f;	// 横：なめらか
	const float rateUp = 0.9f;		// 縦：速く追従

	// --- カメラ位置補間 ---
	pos_ = VAdd(pos_,
		VAdd(
			VAdd(VScale(frontMove, rateFront),
				VScale(sideMove, rateSide)),
			VScale(upMove, rateUp))
	);

	// --- 注視点補間（スムーズに追従） ---
	const float targetRate = 0.2f;
	targetPos_.x += (desiredTargetPos.x - targetPos_.x) * targetRate;
	targetPos_.y += (desiredTargetPos.y - targetPos_.y) * targetRate;
	targetPos_.z += (desiredTargetPos.z - targetPos_.z) * targetRate;

	float dist = VSize(VSub(targetPos_, pos_));
	float adaptiveRate = std::clamp(0.2f + (10000.0f - dist) * 0.002f, 0.1f, 0.4f);
	targetPos_ = VAdd(targetPos_, VScale(VSub(desiredTargetPos, targetPos_), adaptiveRate));

	// --- 上方向 ---
	cameraUp_ = gRot.GetUp();
}

void Camera::SyncFollowFPS(void)
{
	auto gIns = GravityManager::GetInstance();

	// 同期先の位置
	VECTOR pos = followTransform_->pos;

	// 重力の方向制御に従う
	//Quaternion gRot = gIns.GetTransform().quaRot;

	// 正面から設定されたY軸分、回転させる
	//rotOutX_ = gRot.Mult(Quaternion::AngleAxis(angles_.y, Utility::AXIS_Y));

	// 正面から設定されたX軸分、回転させる
	rot_ = rotOutX_.Mult(Quaternion::AngleAxis(angles_.x, Utility::AXIS_X));

	VECTOR localPos;

	// 注視点(通常重力でいうところのY値を追従対象と同じにする)
	//localPos = rotOutX_.PosAxis(LOCAL_F2T_POS);
	localPos = rot_.PosAxis(FPS_LOCAL_F2T_POS);
	targetPos_ = VAdd(pos, localPos);

	// カメラ位置
	localPos = rotOutX_.PosAxis(FPS_LOCAL_F2C_POS);
	pos_ = VAdd(pos, localPos);

	// カメラの上方向
	cameraUp_ = rot_.GetUp();
}

void Camera::ProcessRot(void)
{
	auto& ins = KeyConfig::GetInstance();
	auto keyType = KeyConfig::TYPE::ALL;
	if (ins.IsNew(KeyConfig::CONTROL_TYPE::CAMERA_TURN_RIGHT, padNo_, keyType)) { angles_.y += SPEED_MOUSE; }
	if (ins.IsNew(KeyConfig::CONTROL_TYPE::CAMERA_TURN_LEFT, padNo_, keyType)) { angles_.y -= SPEED_MOUSE; }
	//if (ins.IsNew(KeyConfig::CONTROL_TYPE::CAMERA_TURN_UP, padNo_, keyType)) { angles_.x += SPEED_MOUSE; }
	//if (ins.IsNew(KeyConfig::CONTROL_TYPE::CAMERA_TURN_DOWN, padNo_, keyType)) { angles_.x -= SPEED_MOUSE; }

	//if (angles_.x < FPS_LIMIT_X_UP_RAD)
	//{
	//	angles_.x = FPS_LIMIT_X_UP_RAD;
	//}
	//else if (angles_.x > FPS_LIMIT_X_DW_RAD)
	//{
	//	angles_.x = FPS_LIMIT_X_DW_RAD;
	//}
}

void Camera::ProcessRotMachine(void)
{
	auto& ins = KeyConfig::GetInstance();
	auto keyType = KeyConfig::TYPE::ALL;
	if (ins.IsNew(KeyConfig::CONTROL_TYPE::MACHINE_TURN_RIGHT, padNo_, keyType)) 
	{
		angles_.y += rotPow_ * SPEED_ROT; 
	}
	if (ins.IsNew(KeyConfig::CONTROL_TYPE::MACHINE_TURN_LEFT, padNo_, keyType)) 
	{
		angles_.y -= rotPow_ * SPEED_ROT; 
	}
}

void Camera::ProcessZoom(void)
{
	auto& ins = KeyConfig::GetInstance();
	auto keyType = KeyConfig::TYPE::ALL;
	auto vec = VNorm(VSub(LOCAL_F2T_POS,LOCAL_F2C_POS));
	if (ins.IsNew(KeyConfig::CONTROL_TYPE::CAMERA_ZOOM_IN, padNo_, keyType))
	{
		localPos_ = VAdd(localPos_, VScale(vec, ZOOM_SPEED));
	}
	if (ins.IsNew(KeyConfig::CONTROL_TYPE::CAMERA_ZOOM_IN, padNo_, keyType))
	{
		localPos_ = VAdd(localPos_, VScale(VScale(vec,-1), ZOOM_SPEED));
	}
	if (Utility::Distance(LOCAL_F2C_POS, localPos_) > ZOOM_RADIUS)
	{
		vec = VNorm(VSub(localPos_,LOCAL_F2C_POS));
		localPos_ = VAdd(LOCAL_F2C_POS, VScale(vec, ZOOM_RADIUS));
	}
}

void Camera::ProcessRotMause(float* x_m, float* y_m, const float fov_per)
{
	int x_t, y_t;
	GetMousePoint(&x_t, &y_t);
	*x_m += float(std::clamp(x_t - Application::SCREEN_SIZE_X / 2, -120, 120)) * fov_per / GetFPS();
	*y_m += float(std::clamp(y_t - Application::SCREEN_SIZE_Y / 2, -120, 120)) * fov_per / GetFPS();
	SetMousePoint(Application::SCREEN_SIZE_X / 2, Application::SCREEN_SIZE_Y / 2);

	// マウスを表示状態にする
	SetMouseDispFlag(FALSE);

	if (angles_.x <= FPS_LIMIT_X_UP_RAD)
	{
		angles_.x = FPS_LIMIT_X_UP_RAD;
	}
	if (angles_.x >= FPS_LIMIT_X_DW_RAD)
	{
		angles_.x = FPS_LIMIT_X_DW_RAD;
	}
}

void Camera::SetBeforeDrawFixedPoint(void)
{
	rot_ = Quaternion::Quaternion(angles_);

	// 重力の方向制御に従う
	Quaternion gRot = Quaternion::Euler(VECTOR(0.0, 0.0, 0.0));

	// 正面から設定されたY軸分、回転させる
	rotOutX_ = gRot.Mult(Quaternion::AngleAxis(angles_.y, Utility::AXIS_Y));
}


void Camera::SetBeforeDrawFollow(void)
{
	// カメラ操作
	ProcessRot();

	// 追従対象との相対位置を同期
	SyncFollow();
}

void Camera::SetBeforeDrawFollowLeap(void)
{
	// カメラ操作
	ProcessRotMachine();

	//追従対象を遅れて追尾
	SyncFollowLeap();
}

void Camera::SetBeforeDrawFollowRotation(void)
{
	const float SPEED_DEG = 1.5f;
	angles_.y += Utility::Deg2RadF(SPEED_DEG);

	auto vec = VNorm(VSub(LOCAL_F2T_POS, LOCAL_F2C_POS));
	localPos_ = VAdd(localPos_, VScale(vec, ZOOM_SPEED));
	if (Utility::Distance(LOCAL_F2C_POS, localPos_) > ZOOM_RADIUS)
	{
		vec = VNorm(VSub(localPos_, LOCAL_F2C_POS));
		localPos_ = VAdd(LOCAL_F2C_POS, VScale(vec, ZOOM_RADIUS));
	}

	// 追従対象との相対位置を同期
	SyncFollow();
}

void Camera::SetBeforeDrawSelfShot(void)
{
	auto gIns = GravityManager::GetInstance();

	// 同期先の位置
	VECTOR pos = followTransform_->pos;


	// 正面から設定されたX軸分、回転させる
	rot_ = rotOutX_.Mult(Quaternion::AngleAxis(angles_.x, Utility::AXIS_X));

	VECTOR localPos;

	// 注視点(通常重力でいうところのY値を追従対象と同じにする)
	localPos = rotOutX_.PosAxis(LOCAL_F2T_POS);
	targetPos_ = VAdd(pos, localPos);

	// カメラ位置
	localPos = rot_.PosAxis(LOCAL_F2C_POS);
	pos_ = VAdd(pos, localPos);


}

void Camera::SetBeforeDrawFPS(void)
{
	//マウスでのカメラ操作
	ProcessRotMause(&angles_.y, &angles_.x, 0.2f);

	// 追従対象との相対位置を同期
	SyncFollowFPS();
}

void Camera::SetBeforeDrawFreeControll(void)
{
	auto& ins = KeyConfig::GetInstance();
	float rotPow = Utility::Deg2RadF(SPEED);
	auto keyType = KeyConfig::TYPE::ALL;
	//if (ins.IsNew(KeyConfig::CONTROL_TYPE::EDIT_CAMERA_ROT_RIGHT, padNo_,keyType)) { angles_.y += rotPow; }
	//if (ins.IsNew(KeyConfig::CONTROL_TYPE::EDIT_CAMERA_ROT_LEFT, padNo_,keyType)) { angles_.y -= rotPow; }
	//if (ins.IsNew(KeyConfig::CONTROL_TYPE::EDIT_CAMERA_ROT_UP, padNo_,keyType)) { angles_.x -= rotPow; }
	//if (ins.IsNew(KeyConfig::CONTROL_TYPE::EDIT_CAMERA_ROT_DOWN, padNo_,keyType)) { angles_.x += rotPow; }

	if (angles_.x <= FPS_LIMIT_X_UP_RAD)
	{
		angles_.x = FPS_LIMIT_X_UP_RAD;
	}
	if (angles_.x >= FPS_LIMIT_X_DW_RAD)
	{
		angles_.x = FPS_LIMIT_X_DW_RAD;
	}
	
	auto rStick = ins.GetKnockRStickSize(padNo_);
	rotPow = SPEED_PAD;
	angles_.x += Utility::Deg2RadF(rStick.y *rotPow);
	angles_.y += Utility::Deg2RadF(rStick.x *rotPow);

	//if (ins.IsNew(KEY_INPUT_W)) 
	//{
	//	pos_ = VAdd(pos_, VScale(Quaternion::Quaternion(angles_).GetForward(), 3.0f));
	//}
	//if (ins.IsNew(KEY_INPUT_S))
	//{
	//	pos_ = VAdd(pos_, VScale(Quaternion::Quaternion(angles_).GetBack(), moveSpeed));
	//}
	static float moveSpeed = 10.0f;
	static float moveSpeedFB = 30.0f;
	//pos_ = VAdd(pos_, VScale(Quaternion::Quaternion(angles_).GetForward(), ins.IsNew(KeyConfig::CONTROL_TYPE::EDIT_CAMERA_MOVE_FRONT, padNo_,keyType) * moveSpeedFB));
	//pos_ = VAdd(pos_, VScale(Quaternion::Quaternion(angles_).GetBack(), ins.IsNew(KeyConfig::CONTROL_TYPE::EDIT_CAMERA_MOVE_BACK, padNo_,keyType) * moveSpeedFB));
	//if (ins.IsNew(KeyConfig::CONTROL_TYPE::EDIT_CAMERA_MOVE_LEFT, padNo_,keyType))
	//{
	//	pos_ = VAdd(pos_, VScale(Quaternion::Quaternion(angles_).GetLeft(), moveSpeed));
	//}
	//if (ins.IsNew(KeyConfig::CONTROL_TYPE::EDIT_CAMERA_MOVE_RIGHT, padNo_,keyType))
	//{
	//	pos_ =VAdd(pos_, VScale(Quaternion::Quaternion(angles_).GetRight(), moveSpeed));
	//}
	//if (ins.IsNew(KeyConfig::CONTROL_TYPE::EDIT_CAMERA_MOVE_UP, padNo_,keyType))
	//{
	//	pos_ =VAdd(pos_, VScale(Quaternion::Quaternion(angles_).GetUp(), moveSpeed));
	//}
	//if (ins.IsNew(KeyConfig::CONTROL_TYPE::EDIT_CAMERA_MOVE_DOWN, padNo_,keyType))
	//{
	//	pos_ =VAdd(pos_, VScale(Quaternion::Quaternion(angles_).GetDown(), moveSpeed));
	//}

	VECTOR localPos;
	rot_ =(Quaternion::Quaternion(angles_));
	// 注視点(通常重力でいうところのY値を追従対象と同じにする)
	localPos = rot_.PosAxis(LOCAL_F2T_POS);
	targetPos_ = VAdd(pos_, localPos);


}

void Camera::SetBeforeDrawFixedUp(void)
{
	targetPos_ = VAdd(pos_, FIXED_LOCAL_P2T_POS);
	rot_ = Quaternion::Quaternion(angles_);
	cameraUp_ = rot_.GetUp();
}

void Camera::SetBeforeDrawFixedDiagonal(void)
{
	targetPos_ = FIXED_DIAGONAL_TARGET_POS;
	rot_ = Quaternion::Quaternion(angles_);
	cameraUp_ = rot_.GetUp();
}
