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
	order_ = ORDER::THINK;

	//判断基準
	priorityJudge_.push_back([this](int _num) {JudgeItemBox(_num); });
	priorityJudge_.push_back([this](int _num) {JudgeItem(_num); });

	//更新
	update_.emplace(ORDER::THINK, [this](void) {UpdateThink(); });
	update_.emplace(ORDER::ACTION, [this](void) {UpdateAction(); });
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
	isEndAction_ = true;
	order_ = ORDER::THINK;
}

void NpcBrain::Update(void)
{
	update_[order_]();
}

void NpcBrain::UpdateThink(void)
{
	//サイズ
	int size = priorityJudge_.size();

	//行動決め
	for (int i = 0; i < JUDGE_NUM; i++)
	{
		priorityJudge_[Utility::GetRandomValue(0, size - 1)](i);
		//JudgeItemBox(i);
	}
}

void NpcBrain::UpdateAction(void)
{

}

void NpcBrain::JudgeItemBox(int _num)
{
	//インスタンス
	const auto& itemMng = ItemManager::GetInstance();

	//座標
	std::vector<VECTOR> poses = itemMng.GetItemBoxPos();
	const VECTOR parentPos = parent_.GetTrans().pos;

	//大きさ
	int size = poses.size();
	if (size < 1)
	{
		//アイテムに移す
		JudgeItem(_num);
		return;
	}

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
	simulation_[_num].moveVec = Utility::GetMoveVec(parentPos, poses[rand]);
}

void NpcBrain::JudgeItem(int _num)
{
	//インスタンス
	const auto& itemMng = ItemManager::GetInstance();
}