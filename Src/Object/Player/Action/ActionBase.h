#pragma once

#include"../Player.h"
#include"../Logic/LogicBase.h"

class ActionBase
{
public:

	/// @brief コンストラクタ
	/// @param _player 親情報
	/// @param _logic 行動情報
	ActionBase(Player& _player, LogicBase& _logic);

	//デストラクタ
	virtual ~ActionBase(void);

	//初期化
	virtual void Init(void);

	//更新
	virtual void Update(void);

	//描画
	virtual void Draw(void);

	//X軸回転の初期化
	void ResetAxisX(void) { axis_.x = 0.0f; };

protected:

	//所有者
	Player& player_;

	//行動判断
	LogicBase& logic_;

	//軸ごとの回転用
	VECTOR axis_;
};