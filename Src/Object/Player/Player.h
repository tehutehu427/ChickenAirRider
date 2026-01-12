#pragma once
#include <map>
#include <functional>
#include "../ObjectBase.h"
#include "Parameter/Parameter.h"
#include "../Manager/System/KeyConfig.h"

class Character;
class Machine;
class LogicBase;
class ActionBase;
class PlayerOnHit;
class Camera;

class Player : public ObjectBase, public std::enable_shared_from_this<Player>
{
public:

	//操作タイプ
	enum class OPERATION_TYPE
	{
		USER,	//人
		NPC,	//コンピューター
	};

	//現在の状態
	enum class STATE
	{
		NONE = -1,

		NORMAL,			//生身
		RIDE_MACHINE,	//機体乗車中
	};

	//当たり判定添え字
	enum class COL_VALUE
	{
		MAIN,			//本体
		GROUNDED_PRE,	//前接地
		GROUNDED_OLD,	//後接地
	};

	//コンストラクタ
	Player(const int _plIndex, std::weak_ptr<Camera> _camera, OPERATION_TYPE _operation, KeyConfig::JOYPAD_NO _padNo, Collider::TAG _playerTag);
	//デストラクタ
	~Player(void);

	//読み込み
	void Load(void)override;

	//初期化
	void Init(void)override;
	
	//更新
	void Update(void)override;
	
	//描画
	void Draw(void)override;

	//成長パラメーターの描画
	void DrawGrowthParam(void);

	//当たり判定処理
	void OnHit(const std::weak_ptr<Collider> _hitCol)override;

	//パッド番号の取得
	const KeyConfig::JOYPAD_NO GetPadNo(void)const { return padNo_; }

	//パラメーターの取得
	const Parameter& GetParam(void)const { return param_; }

	//パラメーターの設定
	void SetParam(const Parameter& _param);

	//総合パラメーター
	const Parameter GetAllParam(void)const;

	//攻撃パラメーター
	const float GetAttack(void)const;

	//防御パラメーター
	const float GetDefence(void)const;

	//ユニットのみのパラメーター
	UnitParameter GetUnitParam(void)const;

	//状態
	const STATE GetState(void) { return state_; }

	//回転の設定
	void SetQuaRot(const Quaternion& _quaRot) { trans_.quaRot = _quaRot; }

	//大きさの設定
	void SetScale(const VECTOR _scale) { trans_.scl = _scale; }

	//移動力取得
	const VECTOR& GetMovePow(void)const { return movePow_; }

	//移動力変更
	void SetMovePow(const VECTOR& _movePow) { movePow_ = _movePow; }

	//行動切り替え
	void ChangeState(const STATE& _state);
	
	//接地判定
	const bool IsGrounded(void) { return isGrounded_; }

	//接地判定の切り替え
	void SetIsGrounded(const bool _isGrounded) { isGrounded_ = _isGrounded; }

	//スピン判定の取得
	const bool IsSpin(void)const { return isSpin_; }

	//スピン判定の設定
	void SetIsSpin(const bool _isSpin);

	//移動前座標の取得
	const VECTOR& GetPrePos(void) { return prePos_; }

	//移動前座標の設定
	void SetPrePos(const VECTOR _pos) { prePos_ = _pos; }

	//移動後座標の取得
	const VECTOR& GetMovedPos(void) { return movedPos_; }

	//移動後座標の設定
	void SetMovedPos(const VECTOR _pos) { movedPos_ = _pos; }

	//足元方向の取得
	const Quaternion& GetFoot(void) { return footLine_; }

	//足元方向の設定
	void SetFootLine(const Quaternion _foot) { footLine_ = _foot; }

	//現在体力を返す
	const float GetNowHealth(void)const { return GetAllParam().maxHealth_ - damage_; }

	//カメラの取得
	std::weak_ptr<Camera> GetCamera(void)const { return camera_; }

	//基本機能を返す
	std::weak_ptr<ActionBase> GetAction(void)const { return action_; }
	LogicBase& GetLogic(void)const { return *logic_; }
	Character& GetChara(void)const { return *chara_; }
	//PlayerUI& GetUI(void)const { return *ui_; }

	//機体に乗る
	void RideMachine(std::unique_ptr<Machine> _machine);

	//ダメージ処理
	void Damage(const float _damage);

	//プレイヤー番号の取得
	const int GetPlayerIndex(void)const { return playerIndex_; }

	//プレイヤーの操作者
	const OPERATION_TYPE GetOperation(void) { return operation_; }

private:

	//相対座標
	static constexpr VECTOR LOCAL_POS = { 100.0f,0.0f,0.0f };

	//当たり判定用
	static constexpr float NORMAL_RADIUS = 25.0f;							//通常時の球体の半径
	static constexpr float SPIN_RADIUS = 90.0f;								//スピンの球体の半径
	static constexpr float SPIN_SPEED = 40.0f;								//スピンの速度
	static constexpr VECTOR LOCAL_LINE_UP = { 0.0f,0.0f,0.0f };				//線判定の上相対座標
	static constexpr VECTOR LOCAL_LINE_DOWN = { 0.0f,-40.0f,0.0f };			//線判定の下相対座標

	//基本機能
	std::unique_ptr<Character> chara_;		//キャラクタ―
	std::unique_ptr<Machine> machine_;		//機体
	std::unique_ptr<LogicBase> logic_;		//行動操作者
	std::shared_ptr<ActionBase> action_;	//行動
	std::unique_ptr<PlayerOnHit> onHit_;	//当たり判定
	//std::unique_ptr<PlayerUI> ui_;			//UI

	//プレイヤー番号
	const int playerIndex_;

	//プレイヤーごとのタグ
	Collider::TAG playerTag_;

	//カメラ
	std::weak_ptr<Camera> camera_;

	//操作タイプ
	OPERATION_TYPE operation_;

	//パッド番号
	KeyConfig::JOYPAD_NO padNo_;

	//パラメーター
	Parameter param_;

	//状態
	STATE state_;

	//移動前座標
	VECTOR prePos_;

	//移動後座標
	VECTOR movedPos_;

	//前後座標
	VECTOR flontPos_;
	VECTOR backPos_;

	//移動力
	VECTOR movePow_;

	//足元
	Quaternion footLine_;

	//モデル回転用
	Quaternion modelQuaRot_;

	//接地しているか
	bool isGrounded_;

	//スピンしているか
	bool isSpin_;

	//現在ダメージ
	float damage_;

	//無敵時間
	float invincible_;

	//関数ポインタ
	std::map<OPERATION_TYPE, std::function<void(void)>> createLogic_;	//操作
	std::map<STATE, std::function<void(void)>> changeAction_;			//行動切り替え
	std::map<STATE, std::function<void(void)>> update_;					//状態ごとの更新
	std::map<STATE, std::function<void(void)>> draw_;					//状態ごとの描画

	//状態ごとのアクションの変更
	void ChangeActionNormal(void);
	void ChangeActionRide(void);

	//状態ごとの更新
	void UpdateNormal(void);
	void UpdateRide(void);

	//状態ごとの描画
	void DrawNormal(void);
	void DrawRide(void);

	//キャラと座標と回転の同期する
	void SynchronizeChara(void);

	//機体と座標と回転の同期する
	void SynchronizeMachine(void);
};

