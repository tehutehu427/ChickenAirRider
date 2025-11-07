#pragma once
#include "LogicBase.h"
#include "../Common/Vector2F.h"

class PlayerLogic : public LogicBase
{
public:

	//コンストラクタ
	PlayerLogic(void);

	//デストラクタ
	~PlayerLogic(void)override;

#pragma region 機体

	/// @brief プッシュしたか
	/// @param  
	/// @return プッシュの有無(true:プッシュした)
	const bool IsPush(void) override;

	/// @brief チャージ開始
	/// @param  
	/// @return チャージ開始の有無(true:チャージ開始)
	const bool StartCharge(void) override;

	/// @brief チャージ解放
	/// @param  
	/// @return チャージ解放の有無(true:チャージ解放)
	const bool DisCharge(void) override;

	/// @brief ターンの値
	/// @param  
	/// @return 回転量(-値:左回転, +値:右回転)
	const Vector2F TurnValue(void) override;

	/// @brief スペシャルボタンを押した
	/// @param  
	/// @return スペシャルの有無(true:スペシャルを押した)
	const bool IsSpecial(void) override;

	/// @brief 機体から降りたか
	/// @param  
	/// @return 機体からの降下の有無(true:降りた)
	const bool IsGetOff(void) override;

#pragma endregion 機体

#pragma region キャラクター

	/// @brief 歩く
	/// @param  
	/// @return 歩きの値
	const Vector2F WalkValue(void)override;

	/// @brief ジャンプの有無
	/// @param  
	/// @return true:ジャンプした
	const bool IsJump(void)override;

#pragma endregion キャラクター
		
private:

	//回転量
	static constexpr float TURN_POW = 1.0f;

	//チャージまでのプッシュ時間
	static constexpr float CHARGE_START_PUSH_TIME = 0.1f;

	//機体を降りるまでのスペシャルボタン押下時間
	static constexpr float GETOFF_PUSH_TIME = 1.0f;

	//移動量
	static constexpr float MOVE_POW = 1.0f;
};

