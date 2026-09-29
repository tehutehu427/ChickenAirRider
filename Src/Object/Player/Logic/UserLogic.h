#pragma once
#include "LogicBase.h"
#include "../Common/Vector2F.h"

class UserLogic : public LogicBase
{
public:

	//機体を降りるまでのスペシャルボタン押下時間
	static constexpr float GETOFF_PUSH_TIME = 1.5f;

	//コンストラクタ
	UserLogic(KeyConfig::JOYPAD_NO _padNo);

	//デストラクタ
	~UserLogic(void)override;

	//初期化
	void Init(void)override;

	//更新
	void Update(void)override;

#pragma region 機体

	//プッシュしたか
	const bool IsPush(void)const override;

	///チャージ開始したか
	const bool StartCharge(void)const override;

	//チャージ解放したか
	const bool DisCharge(void)const override;

	//ターンの値(-値:左回転, +値:右回転)
	const Vector2F TurnValue(void)const override;

	//スペシャルボタンを押したか
	const bool IsSpecial(void)const override;

	//機体から降りたか
	const bool IsGetOff(void)const override;

	//レバガチャ判定
	const bool IsButtonMeshing(void)override;

#pragma endregion 機体

#pragma region キャラクター

	//歩きの値
	const Vector2F WalkValue(void)const override;

	//ジャンプしたか
	const bool IsJump(void)const override;

#pragma endregion キャラクター
		
private:

	//回転量
	static constexpr float TURN_STICK = 500.0f;
	static constexpr float TURN_MOUSE = 25.0f;

	//チャージまでのプッシュ時間
	static constexpr float CHARGE_START_PUSH_TIME = 0.1f;

	//移動量
	static constexpr float MOVE_POW = 1.0f;

	//レバガチャ数
	static constexpr int BUTTON_MESHING_MAX = 2;

	//レバガチャ受付時間
	static constexpr float BUTTON_MESHING_RIMIT = 0.25f;

	//パッド番号
	KeyConfig::JOYPAD_NO padNo_;

	//一フレーム前の回転量
	Vector2F oldTurnValue_;

	//新しい回転量
	Vector2F newTurnValue_;

	//レバガチャカウンタ
	int buttonMeshingCnt_;

	//カウンタ
	float cnt_;
};