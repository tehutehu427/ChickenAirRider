#pragma once
#include<DxLib.h>
#include"../Common/Vector2F.h"

struct SimulationResult
{
	//スコア
	int priority = 0;
	
	//移動方向
	Vector2F moveVec = { 0.0f,0.0f };

	//予想最終地点
	VECTOR endPoint = { 0.0f,0.0f,0.0f };
};