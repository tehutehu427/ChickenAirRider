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

	//初期ジャンプ量
	static constexpr float JUMP_POW = 100.0f;

	//移動量
	static constexpr float WALK_POW = 10.0f;

	//キャラクター
	Character& chara_;

	//移動量
	VECTOR walkPow_;

	//ジャンプ量
	VECTOR jumpPow_;

	//向く方向
	Quaternion rot_;

	//状態ごとの更新
	void UpdateGround(void)override;
	void UpdateFlight(void)override;

	//歩き
	void Walk(void);

	//ジャンプ
	void Jump(void);

	//重力
	void CalcGravity(void);
};

