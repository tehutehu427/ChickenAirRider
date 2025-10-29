#include"../pch.h"
#include"../Manager/System/KeyConfig.h"
#include"../Manager/System/SceneManager.h"
#include "SceneSelect.h"

SceneSelect::SceneSelect(void)
{
}

SceneSelect::~SceneSelect(void)
{
}

void SceneSelect::Load(void)
{
}

void SceneSelect::Init(void)
{
}

void SceneSelect::Update(void)
{


	//シーン遷移デバッグ
	if (KeyConfig::GetInstance().IsTrgDown(KeyConfig::CONTROL_TYPE::ENTER, KeyConfig::JOYPAD_NO::PAD1))
	{
		//シーンの削除
		SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::GAME,true, true);
		return;
	}
}

void SceneSelect::Draw(void)
{
#ifdef _DEBUG

	//デバッグ描画
	DebugDraw();

#endif // _DEBUG
}

void SceneSelect::Release(void)
{
}

void SceneSelect::DebugDraw(void)
{
	//シーン名
	DrawString(0, 0, L"SceneSelect", 0xffffff);

	DrawBox(100, 100, 924, 540, 0x00ff0f, true);
}
