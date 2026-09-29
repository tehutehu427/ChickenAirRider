#include"../pch.h"
#include "../Utility/Utility.h"
#include "Grid.h"

Grid::Grid(void)
{
}

Grid::~Grid(void)
{
}

void Grid::Draw(void)
{
	//半透明に設定
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, Utility::ALPHA_MAX / 2);	

	// XZ基本軸(グリッド)
	VECTOR sPos;
	VECTOR ePos;
	IntVector3 lineNum = { 10 ,3 ,100 };
	int size = 50;
	for (int i = 0; i <= lineNum.x; i++)
	{
		for (int j = 0; j <= lineNum.y; j++)
		{
			sPos = {static_cast<float>( i * size),static_cast<float>(j * size),0 };
			ePos = {static_cast<float>( i * size),static_cast<float>(j * size),static_cast<float>(lineNum.z * size) };

			DrawLine3D(sPos, ePos,Utility::RED);
		}
	}
	for (int i = 0; i <= lineNum.x; i++)
	{
		for (int j = 0; j <= lineNum.z; j++)
		{
			sPos = {static_cast<float>( i * size),0,static_cast<float>(j * size) };
			ePos = {static_cast<float>( i * size),static_cast<float>(lineNum.y * size),static_cast<float>(j * size) };

			DrawLine3D(sPos, ePos, Utility::GREEN);
		}
	}
	for (int i = 0; i <= lineNum.y; i++)
	{
		for (int j = 0; j <= lineNum.z; j++)
		{
			sPos = {0,static_cast<float>( i * size),static_cast<float>(j * size) };
			ePos = {static_cast<float>(lineNum.x * size),static_cast<float>( i * size),static_cast<float>(j * size) };

			DrawLine3D(sPos, ePos, Utility::BLUE);
		}
	}
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);	//描画モードを元に戻す
}