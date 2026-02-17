#include "../pch.h"
#include "../Utility/Utility.h"
#include "../Manager/Game/ItemManager.h"
#include "NpcBrain.h"

NpcBrain::NpcBrain(const Player& _parent)
	: parent_(_parent)
{
	for (int i = 0; i < JUDGE_NUM; i++)
	{
		actionJudge_[i] = 0;
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
		actionJudge_[i] = 0;
	}
	isEndAction_ = false;
}

void NpcBrain::Update(void)
{
}

void NpcBrain::JudgeItemBox(void)
{
	//インスタンス
	const auto& itemMng = ItemManager::GetInstance();

	//座標
	std::vector<VECTOR> poses = itemMng.GetItemBoxPos();
	int size = poses.size();
	VECTOR parentPos = parent_.GetTrans().pos;

	//乱数
	int rand = Utility::GetRandomValue(0, size);
	float randDist = Utility::Distance(parentPos, poses[rand]);

	for (int i = 0; i < size; i++)
	{
		
	}
}

void NpcBrain::JudgeItem(void)
{
}
