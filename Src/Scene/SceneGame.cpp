#include<DxLib.h>
#include "SceneGame.h"

SceneGame::SceneGame(void)
{
}

SceneGame::~SceneGame(void)
{
}

void SceneGame::Load(void)
{
}

void SceneGame::Init(void)
{
}

void SceneGame::Update(void)
{
}

void SceneGame::Draw(void)
{
#ifdef _DEBUG

	//デバッグ描画
	DebugDraw();

#endif // _DEBUG

}

void SceneGame::Release(void)
{
}

void SceneGame::DebugDraw(void)
{
	//シーン名
	DrawString(0, 0, L"SceneGame", 0xffffff);
}
