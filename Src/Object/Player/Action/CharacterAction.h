#pragma once
#include<unordered_map>
#include<string>
#include"ActionBase.h"
#include"../Character/Character.h"

class CharacterAction : public ActionBase
{
public:

	/// @brief コンストラクタ
	/// @param _player 親情報
	/// @param _chara キャラクターの情報
	/// @param _logic 行動情報
	CharacterAction(Player& _player, Character& _chara, LogicBase& _logic);

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
	Character& chara_;

	//アニメーション番号
	std::unordered_map<std::string, int> animNum_;
};

