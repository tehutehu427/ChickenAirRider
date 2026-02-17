#pragma once
#include"../Object/Player/Player.h"

class NpcBrain
{
public:

	//コンストラクタ
	NpcBrain(const Player& _parent);

	//デストラクタ
	~NpcBrain(void);

	//初期化
	void Init(void);

	//更新
	void Update(void);

private:

	//行動判断回数
	static constexpr int JUDGE_NUM = 10;

	//親
	const Player& parent_;

	//行動判断用の重み付け
	int actionJudge_[JUDGE_NUM];

	//行動を終えたか
	bool isEndAction_;

	//要因ごとの判断
	void JudgeItemBox(void);
	void JudgeItem(void);
};

