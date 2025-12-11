#include"../pch.h"
#include"../Application.h"
#include"../Manager/System/KeyConfig.h"
#include"../Manager/System/SceneManager.h"
#include "../Manager/System/ResourceManager.h"
#include "../Renderer/PixelMaterial.h"
#include "../Renderer/PixelRenderer.h"
#include "SceneSelect.h"
#include "SceneTitle.h"

SceneTitle::SceneTitle(void)
{
	backImg_ = -1;
	logoImg_ = -1;
}

SceneTitle::~SceneTitle(void)
{
	DeleteGraph(postEffectScreen_);
}

void SceneTitle::Load(void)
{
	//画像の読み込み
	backImg_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::TITLE_BACK).handleId_;
	logoImg_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::TITLE_LOGO).handleId_;


	//material_ = std::make_unique<PixelMaterial>(L"Title.cso", 1);
	//renderer_ = std::make_unique<PixelRenderer>(*material_);

	//material_->AddConstBuf(FLOAT4{ 0.1f, 0.0f,0.98f, 0.7f });
	//material_->AddTextureBuf(SceneManager::GetInstance().GetMainScreen());
	//renderer_->MakeSquereVertex({ 0,0 }, { Application::SCREEN_SIZE_X, Application::SCREEN_SIZE_Y });

	//postEffectScreen_ = MakeScreen(Application::SCREEN_SIZE_X, Application::SCREEN_SIZE_Y, true);
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

	// ポストエフェクトの描画
	//SetDrawScreen(postEffectScreen_);
	//ClearDrawScreen();
	//material_->SetTextureBuf(0, SceneManager::GetInstance().GetMainScreen());
	//renderer_->Draw();
	//SetDrawScreen(SceneManager::GetInstance().GetMainScreen());
	//DrawGraph(0, 0, postEffectScreen_, true);
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
