#pragma once
#include "../LogicBase.h"
#include "../Common/Vector2F.h"
#include "../Object/Player/Player.h"

class NpcBrain;

class NpcLogic : public LogicBase
{
public:

	//コンストラクタ
	NpcLogic(const Player& _parent);

	//デストラクタ
	~NpcLogic(void)override;

	//初期化
	void Init(void)override;

	//更新
	void Update(void)override;

#pragma region 機体

	//プッシュしたか
	const bool IsPush(void)const override;

	//チャージ開始したか
	const bool StartCharge(void)const override;

	//チャージ解放したか
	const bool DisCharge(void)const override;

	//ターンの値(-値:左回転, +値:右回転)
	const Vector2F TurnValue(void)const override;

	//スペシャルボタンを押したか
	const bool IsSpecial(void)const override;

	//機体から降りたか
	const bool IsGetOff(void)const override;

	//レバガチャ判定
	const bool IsButtonMeshing(void)override;

#pragma endregion 機体

#pragma region キャラクター

	//歩きの値
	const Vector2F WalkValue(void)const override;

	//ジャンプしたか
	const bool IsJump(void)const override;

#pragma endregion キャラクター

	//セッター
	void SetIsPush(const bool _value) { push_ = _value; }
	void SetIsStartCharge(const bool _value) { startCharge_ = _value; }
	void SetIsDisCharge(const bool _value) { disCharge_ = _value; }
	void SetTurnValue(const Vector2F _value) { turnValue_ = _value; }
	void SetIsSpecial(const bool _value) { special_ = _value; }
	void SetIsGetOff(const bool _value) { getOff_ = _value; }
	void SetIsButtonMeshing(const bool _value) { buttonMeshing_ = _value; }
	void SetWalkValue(const Vector2F _value) { walkValue_ = _value; }
	void SetIsJump(const bool _value) { jump_ = _value; }

private:

	//行動判断
	std::unique_ptr<NpcBrain> brain_;

	//親
	const Player& parent_;

	//各行動フラグ
	bool push_;
	bool startCharge_;
	bool disCharge_;
	Vector2F turnValue_;
	bool special_;
	bool getOff_;
	bool buttonMeshing_;
	Vector2F walkValue_;
	bool jump_;
};

