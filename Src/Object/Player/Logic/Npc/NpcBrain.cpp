#include "../pch.h"
#include "../Utility/Utility.h"
#include "../Manager/Game/ItemManager.h"
#include "../Manager/Game/PlayerManager.h"
#include "../Manager/Game/GameSetting.h"
#include "NpcBrain.h"

NpcBrain::NpcBrain(const Player& _parent, NpcLogic& _logic)
	: parent_(_parent),
	logic_(_logic)
{
	for (int i = 0; i < JUDGE_NUM; i++)
	{
		simulation_[i] = {};
	}
	isEndAction_ = false;
}

NpcBrain::~NpcBrain(void)
{
}

void NpcBrain::Init(void)
{
	//初期化
	for (int i = 0; i < JUDGE_NUM; i++)
	{
		simulation_[i] = {};
	}
	isEndAction_ = false;
}

void NpcBrain::Update(void)
{
	for (int i = 0; i < JUDGE_NUM; i++)
	{
		JudgeItemBox(i);
	}
}

void NpcBrain::JudgeItemBox(int _num)
{
	//インスタンス
	const auto& itemMng = ItemManager::GetInstance();

	//座標
	std::vector<VECTOR> poses = itemMng.GetItemBoxPos();
	int size = poses.size();
	if (size < 1)return;
	const VECTOR parentPos = parent_.GetTrans().pos;

	//乱数
	int rand = Utility::GetRandomValue(0, size - 1);

	//距離
	float randDist = Utility::Distance(parentPos, poses[rand]);
	float dist = 0.0f;

	//優先度
	int priority = 0;

	for (int i = 0; i < size; i++)
	{
		//ボックスの距離
		dist = Utility::Distance(parentPos, poses[i]);
		
		//近いなら優先度をあげる
		if (randDist > dist)priority += DIST_WEIGHT;
	}

	//対戦相手と比べた距離を見る
	auto& plMng = PlayerManager::GetInstance();
	const auto& set = GameSetting::GetInstance();
	int plNum = set.GetPlayerNum();

	for (int i = 0; i < plNum; i++)
	{
		//相手側のボックスとの距離
		float enemyDist = Utility::Distance(plMng.GetPlayer(i).GetTrans().pos,poses[rand]);

		//相手より遠いなら優先度を下げる
		if (randDist < enemyDist)priority -= ENEMY_WEIGHT;
	}

	//代入
	simulation_[_num].priority = priority;
	simulation_[_num].endPoint = poses[rand];
	simulation_[_num].moveVec = Utility::GetMoveVec()
}

void NpcBrain::JudgeItem(int _num)
{
}
