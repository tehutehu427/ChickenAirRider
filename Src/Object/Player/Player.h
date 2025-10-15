#pragma once
#include <map>
#include <functional>
#include "../ObjectBase.h"
#include "Parameter/Parameter.h"

class Character;
class Machine;
class LogicBase;
class ActionBase;

class Player : public ObjectBase
{
public:

	//現在の状態
	enum class STATE
	{
		NONE = -1,

		NORMAL,			//生身
		RIDE_MACHINE,	//機体乗車中
	};

	//コンストラクタ
	Player(void);
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

	//当たり判定処理
	void OnHit(const std::weak_ptr<Collider> _hitCol)override;

	//総合パラメーター
	const Parameter GetAllParam(void)const;

	//ユニットのみのパラメーター
	UnitParameter GetUnitParam(void)const;

	//回転の設定
	void SetQuaRot(const Quaternion _quaRot) { trans_.quaRot = _quaRot; }

	//移動力変更
	void SetMovePow(const VECTOR _movePow) { movePow_ = _movePow; }

	//行動切り替え
	void ChangeState(const STATE _state) { state_ = _state; }

private:

	//基本機能
	std::unique_ptr<Character> chara_;		//キャラクタ―
	std::unique_ptr<Machine> machine_;		//機体
	std::unique_ptr<LogicBase> logic_;		//行動操作者
	std::unique_ptr<ActionBase> action_;	//行動

	//パラメーター
	Parameter param_;

	//状態
	STATE state_;

	//移動後座標
	VECTOR movedPos_;

	//移動力
	VECTOR movePow_;

	//関数ポインタ
	std::map<STATE, std::function<void(void)>> changeAction_;	//行動切り替え
	std::map<STATE, std::function<void(void)>> draw_;			//状態ごとの描画

	//キャラと座標と回転の同期する
	void SynchronizeChara(void);

	//機体と座標と回転の同期する
	void SynchronizeMachine(void);
};

