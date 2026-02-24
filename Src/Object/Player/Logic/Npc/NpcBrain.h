#pragma once
#include<Queue>
#include"../Object/Player/Player.h"
#include"NpcLogic.h"
#include"SimulationResult.h"

class NpcBrain
{
public:

	//コンストラクタ
	NpcBrain(const Player& _parent, NpcLogic& _logic);

	//デストラクタ
	~NpcBrain(void);

	//初期化
	void Init(void);

	//更新
	void Update(void);

private:

	//行動判断回数
	static constexpr int JUDGE_NUM = 10;

	//重み付け係数
	static constexpr int DIST_WEIGHT = 5;	//距離関係
	static constexpr int ENEMY_WEIGHT = 10;	//対戦相手関係

	//親
	const Player& parent_;
	NpcLogic& logic_;

	//行動判断用の重み付け(優先度)
	SimulationResult simulation_[JUDGE_NUM];

	//要因ごとの優先度決め
	std::queue<std::function<void(void)>> priorityJudge_;
	
	//アクション保存
	std::queue<std::function<void(void)>> action_;

	//行動を終えたか
	bool isEndAction_;

	//要因ごとの判断
	void JudgeItemBox(int _num);
	void JudgeItem(int _num);
};

