#include"../pch.h"
#include"../Application.h"
#include"../Manager/System/KeyConfig.h"
#include"../Manager/System/SceneManager.h"
#include "../Manager/System/ResourceManager.h"
#include "../Manager/System/SoundManager.h"
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
	SoundManager::GetInstance().StopAll();
	DeleteGraph(postEffectScreen_);
}

void SceneTitle::Load(void)
{
	//リソース
	auto& res = ResourceManager::GetInstance();

	//サウンド
	auto& snd = SoundManager::GetInstance();

	//画像の読み込み
	backImg_ = res.Load(ResourceManager::SRC::TITLE_BACK).handleId_;
	logoImg_ = res.Load(ResourceManager::SRC::TITLE_LOGO).handleId_;

	//サウンドの追加
	int id = res.Load(ResourceManager::SRC::ENTER_SE).handleId_;
	snd.Add(SoundManager::SOUND_NAME::ENTER, id, SoundManager::TYPE::SE, 80);

	//id = res.Load(ResourceManager::SRC::TITLE_SE).handleId_;
	//snd.Add(SoundManager::SOUND_NAME::TITLE_SE, id, SoundManager::TYPE::SE, 80);
	//snd.Play(SoundManager::SOUND_NAME::TITLE_SE, SoundManager::PLAYTYPE::LOOP);
}

void SceneTitle::Init(void)
{
}

void SceneTitle::Update(void)
{
	//インスタンス
	auto& snd = SoundManager::GetInstance();
	auto& key = KeyConfig::GetInstance();

	//シーン遷移デバッグ
	if (key.IsTrgDown(KeyConfig::CONTROL_TYPE::ENTER, KeyConfig::JOYPAD_NO::PAD1))
	{
		//決定音
		snd.Play(SoundManager::SOUND_NAME::ENTER, SoundManager::PLAYTYPE::BACK);

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
