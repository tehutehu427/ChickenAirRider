#include<DxLib.h>
#include"../Manager/System/KeyConfig.h"
#include"../Manager/System/SceneManager.h"
#include "SceneSelect.h"
#include "SceneTitle.h"

SceneTitle::SceneTitle(void)
{
}

SceneTitle::~SceneTitle(void)
{
}

void SceneTitle::Load(void)
{
}

void SceneTitle::Init(void)
{
	
}

void SceneTitle::Update(void)
{
	//シーン遷移デバッグ
	if (KeyConfig::GetInstance().IsTrgDown(KeyConfig::CONTROL_TYPE::ENTER, KeyConfig::JOYPAD_NO::PAD1))
	{
		//シーンの削除
		SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::SELECT,true);
		return;
	}
}

void SceneTitle::Draw(void)
{
#ifdef _DEBUG

	//デバッグ描画
	DebugDraw();

#endif // _DEBUG
}

void SceneTitle::Release(void)
{
}

void SceneTitle::DebugDraw(void)
{
	//シーン名
	DrawString(0, 0, L"SceneTitle", 0xffffff);

	DrawBox(100, 100, 924, 540, 0xff0ff0, true);
}
