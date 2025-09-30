#pragma once
#include"ActionBase.h"
#include"../Character/Character.h"

class CharacterAction : public ActionBase
{
public:

	//コンストラクタ
	CharacterAction(Player& _player, const Character& _chara, const LogicBase& _logic);

	//コンストラクタ
	~CharacterAction(void)override;

	//初期化
	void Init(void)override;

	//更新
	void Update(void)override;

	//描画
	void Draw(void)override;

private:

	//キャラクター
	const Character& chara_;
};

