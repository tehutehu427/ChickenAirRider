#pragma once
#include "../ObjectBase.h"
#include"../Common/Parameter.h"

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
	const Parameter& GetAllParam(void)const;

	//移動力変更
	void SetMovePow(const VECTOR _movePow) { movePow_ = _movePow; }

private:

	//基本機能
	std::unique_ptr<Character> chara_;		//キャラクタ―
	std::unique_ptr<Machine> machine_;		//機体
	std::unique_ptr<LogicBase> logic_;		//行動操作者
	std::unique_ptr<ActionBase> action_;	//行動

	//パラメーター
	Parameter param_;

	//移動力
	VECTOR movePow_;
};

