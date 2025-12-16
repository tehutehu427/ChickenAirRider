#pragma once
#include <map>
#include <functional>
#include "ActionBase.h"
#include "../Machine/Machine.h"

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

	//スピン時間
	static constexpr float SPIN_TIME = 0.7f;

	//体力
	static constexpr int HEALTH_BOX_LOCAL_POS_X = 10;
	static constexpr int HEALTH_BOX_LOCAL_POS_Y = 0;
	static constexpr int HEALTH_BOX = 20;
	static constexpr int HEALTH_LOCAL = 3;

	//UIの位置(Normalize)
	static constexpr Vector2F HEALTH_POS = { 0.95f,0.6f };
	static constexpr Vector2F CHARGE_POS = { 0.85f,0.8f };

	//ゲージの大きさ
	static constexpr float GAUGE_SIZE = 512.0f;
	static constexpr float GAUGE_SIZE_MULTI = 0.8f;

	//ゲージの位置補正
	static constexpr int GAUGE_LOCAL_POS = 5;
	static constexpr int NUMBER_LOCAL_POS = 40;

	//機体
	const Machine& machine_;

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

	//ゲージ画像
	int gaugeImg_;
	int gaugeMaskImg_;
	int* numImgs_;

	//シェーダー
	std::unique_ptr<PixelMaterial> material_;
	std::unique_ptr<PixelRenderer> renderer_;
	int maskScreen_;
	float gaugeCnt_;

	//ゲージの位置
	Vector2 chargeGaugePos_;

	//状態ごとの更新
	void UpdateGround(void)override;
	void UpdateFlight(void)override;

	//描画関係
	void DrawGauge(void);
	void DrawHealth(void);

	//移動
	void Move(void);

	//チャージ
	void Charge(void);

	//チャージ解放
	void DisCharge(void);

	//旋回
	void Turn(void);

	//飛行
	void Flight(void);
};

