#include"../pch.h"
#include"../Application.h"
#include"../Manager/System/KeyConfig.h"
#include"../Manager/System/SceneManager.h"
#include "../Manager/System/ResourceManager.h"
#include "SceneSelect.h"
#include "SceneTitle.h"

SceneTitle::SceneTitle(void)
{
	backImg_ = -1;
	logoImg_ = -1;
}

SceneTitle::~SceneTitle(void)
{
}

void SceneTitle::Load(void)
{
	//画像の読み込み
	backImg_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::TITLE_BACK).handleId_;
	logoImg_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::TITLE_LOGO).handleId_;
}

void SceneTitle::Init(void)
{
}

void SceneTitle::Update(void)
{
	auto& key = KeyConfig::GetInstance();

	//シーン遷移デバッグ
	if (key.IsTrgDown(KeyConfig::CONTROL_TYPE::ENTER, KeyConfig::JOYPAD_NO::PAD1))
	{
		//シーンの削除
		SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::SELECT,true, true);
		return;
	}
}

void SceneTitle::Draw(void)
{
#ifdef _DEBUG

	//デバッグ描画
	DebugDraw();

#endif // _DEBUG

	//背景
	DrawExtendGraph(0, 0, Application::SCREEN_SIZE_X, Application::SCREEN_SIZE_Y, backImg_, true);

	//ロゴ
	DrawExtendGraph(LOGO_POS_X_1, LOGO_POS_Y_1, LOGO_POS_X_2, LOGO_POS_Y_2, logoImg_, true);
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
