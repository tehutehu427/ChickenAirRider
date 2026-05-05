#pragma once
#include <unordered_map>
#include "ActionBase.h"
#include "../Machine/Machine.h"
#include "../../Common/EffectController.h"

class PixelMaterial;
class PixelRenderer;

class MachineAction : public ActionBase
{
public:

	/// @brief コンストラクタ
	/// @param _player 親情報
	/// @param _machine 機体の情報
	/// @param _logic 行動情報
	MachineAction(Player& _player, const Machine& _machine, LogicBase& _logic);

	//デストラクタ
	~MachineAction(void)override;

	//初期化
	void Init(void)override;

	//更新
	void Update(void)override;

	//描画
	void Draw(void)override;

	//チャージ時間の取得
	const float GetChargeCnt(void)const { return chargeCnt_; }

	//速度の取得
	const float GetSpeed(void)const { return speed_; }

private:
	
	//最高速の基本倍率
	static constexpr float BASE_MAX_SPEED = 10.0f;

	//加速度の固定上昇値
	static constexpr float BASE_ACCELE = 5.0f;

	//チャージの固定上昇値
	static constexpr float BASE_CHARGE = 50.0f;

	//飛行の固定値
	static constexpr float BASE_FLIGHT = 10.0f;

	//重さの固定値
	static constexpr float BASE_WEIGHT = 10.0f;

	//回転量の補正
	static constexpr float COMP_TURN = 500.0f;

	//チャージ時の回転しやすさ
	static constexpr float CHARGE_TURN = 2.0f;

	//チャージ時のモデル変形補正
	static constexpr float CHARGE_MODEL_TRANS = 100.0f;

	//X回転の上下限値
	static constexpr float AXIS_X_LIMIT = 45.0f;

	//プッシュの落下補正
	static constexpr float PUSH_FALL = 20.0f;

	//ブレーキ力
	static constexpr float BRAKE_POW = 100.0f;
	static constexpr float TURN_BRAKE_POW = BRAKE_POW / 5.0f;

	//空中の速度減衰率
	static constexpr float FLIGHT_DECELERATION = 2.0f;

	//スピン
	static constexpr float SPIN_TIME = 0.7f;
	static constexpr float SPIN_SPEED = 40.0f;

	//サウンド関係
	static constexpr int ENGINE_SE_VOL = 140;
	static constexpr int CHARGE_SE_VOL = 80;
	static constexpr int CHARGE_MAX_SE_VOL = 80;
	static constexpr int SPIN_SE_VOL = 130;
	static constexpr int BOOST_SE_VOL = 180;

	//エフェクト関係
	static constexpr VECTOR BOOST_EFF_SCL = {5.0f, 5.0f, 5.0f};
	static constexpr VECTOR CHARGE_EFF_SCL = {10.0f, 10.0f, 10.0f};
	static constexpr VECTOR SPIN_EFF_SCL = {1.0f, 1.0f, 1.0f };

	//機体
	const Machine& machine_;

	//エフェクト
	std::unique_ptr<EffectController> effectController_;

	//エフェクトの配列番号
	std::unordered_map<EffectController::EFF_TYPE, int> effNum_;

	//走行時間カウンタ
	float driveCnt_;

	//チャージカウンタ
	float chargeCnt_;

	//チャージの完了
	bool isChargeMax_;

	//初速度
	float velocity_;

	//総合速度
	float speed_;

	//飛行力
	float flightPow_;

	//地面から離れた瞬間
	bool isFlightNow_;

	//重力
	VECTOR gravPow_;

	//スピン時間カウント
	float spinCnt_;

	//スピン判定
	bool isSpin_;

	//状態ごとの更新
	void UpdateGround(void)override;
	void UpdateFlight(void)override;

	//移動
	void Move(void);

	//チャージ
	void Charge(void);

	//チャージ解放
	void DisCharge(void);

	//スピン
	void Spin(void);

	//旋回
	void Turn(void);

	//飛行
	void Flight(void);
};

