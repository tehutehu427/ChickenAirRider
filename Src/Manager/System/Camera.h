#pragma once
#include <DxLib.h>
#include <memory>
#include "../../Common/Quaternion.h"
#include "KeyConfig.h"

class Transform;

class Camera
{

public:

	// カメラスピード(度)
	static constexpr float SPEED = 1.0f;
	static constexpr float SPEED_PAD = 0.0015f;
	static constexpr float SPEED_MOUSE = 0.05f;
	static constexpr float SPEED_ROT = 10.0f;

	//カメラズームスピード
	static constexpr float ZOOM_SPEED = 5.0f;

	//カメラのズーム範囲
	static constexpr float ZOOM_RADIUS = 300.0f;
	// カメラクリップ：NEAR
	static constexpr float CAMERA_NEAR = 10.0f;

	// カメラクリップ：NEAR
	static constexpr float CAMERA_FAR = 30000.0f;

	// カメラの初期座標
	static constexpr VECTOR DEFAULT_CAMERA_POS = { 0.0f, 100.0f, -500.0f };

	// 追従位置からカメラ位置までの相対座標
	static constexpr VECTOR LOCAL_F2C_POS = { 0.0f, 30.0f, -200.0f };
	static constexpr VECTOR LOCAL_F2C_LEAP_POS = { 0.0f, 150.0f, -200.0f };
	//static constexpr VECTOR LOCAL_F2C_POS = { 0.0f, -1000.0f, -200.0f };

	//FPSの時の相対座標
	static constexpr VECTOR FPS_LOCAL_F2C_POS = { 0.0f, 130.0f, 20.0f };

	//FPS用
	static constexpr VECTOR FPS_LOCAL_F2T_POS = { 0.0f, 0.0f, 2000.0f };

	//固定上用
	static constexpr VECTOR FIXED_LOCAL_P2T_POS = { 0.0f, -1500.0f, 0.0f };

	//斜め上用
	static constexpr VECTOR FIXED_DIAGONAL_TARGET_POS = { 2000.0f, 00.0f, 3000.0f };

	// 追従位置から注視点までの相対座標
	static constexpr VECTOR LOCAL_F2T_POS = { 0.0f, 0.0f, 200.0f };
	static constexpr VECTOR LOCAL_F2T_LEAP_POS = { 0.0f, 0.0f, 200.0f };

	// カメラのX回転上限度角
	static constexpr float LIMIT_X_UP_RAD = 60.0f * (DX_PI_F / 180.0f);
	static constexpr float LIMIT_X_DW_RAD = 15.0f * (DX_PI_F / 180.0f);

	//FPSの上限角
	static constexpr float FPS_LIMIT_X_UP_RAD = -80.0f * (DX_PI_F / 180.0f);
	static constexpr float FPS_LIMIT_X_DW_RAD = 70.0f * (DX_PI_F / 180.0f);

	
	// カメラモード
	enum class MODE
	{
		NONE,
		FIXED_POINT,
		FOLLOW,
		FOLLOW_LEAP,		//遅れてカメラ追従
		FOLLOW_ROTATION,	//ついじゅうしながら自動回転操作不能
		SELF_SHOT,
		FPS,
		FREE_CONTROLL,
		FIXED_UP,
		FIXED_DIAGONAL,	//斜め固定
	};


	/// @brief コンストラクタ
	/// @param _playerNum ユーザーの数
	Camera(int _userNum);

	//デストラクタ
	~Camera(void);

	//初期化
	void Init(void);
	
	//更新
	void Update(void);
	
	//描画前処理
	void SetBeforeDraw(void);
	
	//描画
	void Draw(void);

	// カメラ位置の取得
	VECTOR GetPos(void) const;

	// カメラの操作角度の取得
	VECTOR GetAngles(void) const;

	// カメラの注視点の取得
	VECTOR GetTargetPos(void) const;

	// カメラ角度の取得
	Quaternion GetQuaRot(void) const;

	// X回転を抜いたカメラ角度の取得
	Quaternion GetQuaRotOutX(void) const;

	// カメラの前方方向の取得
	VECTOR GetForward(void) const;

	// カメラの設定
	void CameraSetting(void);

	// カメラモードの取得
	MODE GetMode(void) { return mode_; }

	// カメラモードの設定
	void ChangeMode(MODE mode);

	// 追従対象の設定
	void SetFollow(const Transform* follow);

	//座標の設定
	void SetPos(VECTOR pos) { pos_ = pos; }
	
	//操作角度の設定
	void SetAngles(VECTOR angles) { angles_ = angles; }
	
	//注視点の設定
	void SetTargetPos(VECTOR pos) { targetPos_ = pos; }

	//回転力設定
	void SetRotPow(const float _rotPow) { rotPow_ = _rotPow; }

private:

	// カメラのローカル座標
	VECTOR localPos_;

	// カメラが追従対象とするTransform
	const Transform* followTransform_;

	//PADの番号
	KeyConfig::JOYPAD_NO padNo_;

	// カメラモード
	MODE mode_;

	// カメラの位置
	VECTOR pos_= {0.0f,0.0f,0.0f};

	// カメラ角度(rad)
	VECTOR angles_;

	// X軸回転が無い角度
	Quaternion rotOutX_;

	// カメラ角度
	Quaternion rot_;

	// 注視点
	VECTOR targetPos_;

	// カメラの上方向
	VECTOR cameraUp_;

	//回転力
	float rotPow_;

	//補間した注視点
	VECTOR leapTargetPos_;

	//カメラ速度
	VECTOR vel_;

	// カメラを初期位置に戻す
	void SetDefault(void);

	// 追従対象との位置同期を取る
	void SyncFollow(void);

	// 追従対象との遅延した位置を取る
	void SyncFollowLeap(void);

	//FPS用の位置同期
	void SyncFollowFPS(void);

	// カメラ操作
	void ProcessRot(void);
	void ProcessRotMachine(void);
	void ProcessZoom(void);

	//マウスでのカメラ操作
	void ProcessRotMouse(float* x_m, float* y_m, const float fov_per = 1.0f);

	// モード別更新ステップ
	void SetBeforeDrawFixedPoint(void);
	void SetBeforeDrawFollow(void);
	void SetBeforeDrawFollowLeap(void);
	void SetBeforeDrawFollowRotation(void);
	void SetBeforeDrawSelfShot(void);
	void SetBeforeDrawFPS(void);
	void SetBeforeDrawFreeControll(void);
	void SetBeforeDrawFixedUp(void);
	void SetBeforeDrawFixedDiagonal(void);
};

