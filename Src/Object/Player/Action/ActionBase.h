#pragma once

#include"../Player.h"
#include"../Logic/LogicBase.h"

class ActionBase
{
public:

	//コンストラクタ
	ActionBase(Player& _player, LogicBase& _logic);

	//コンストラクタ
	virtual ~ActionBase(void);

	//初期化
	virtual void Init(void);

	//更新
	virtual void Update(void);

	//描画
	virtual void Draw(void);

protected:

	//所有者
	Player& player_;

	//行動判断
	LogicBase& logic_;
};