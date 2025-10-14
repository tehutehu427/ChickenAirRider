#pragma once
#include"ActionBase.h"
#include"../Character/Character.h"

class CharacterAction : public ActionBase
{
public:

	/// @brief コンストラクタ
	/// @param _player 親情報
	/// @param _chara キャラクターの情報
	/// @param _logic 行動情報
	CharacterAction(Player& _player, const Character& _chara, LogicBase& _logic);

	//デストラクタ
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

