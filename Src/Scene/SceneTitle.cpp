#include"../pch.h"
#include"../Application.h"
#include"../Utility/Utility.h"
#include"../Manager/System/KeyConfig.h"
#include"../Manager/System/SceneManager.h"
#include "../Manager/System/ResourceManager.h"
#include "../Manager/System/SoundManager.h"
#include "../Renderer/PixelMaterial.h"
#include "../Renderer/PixelRenderer.h"
#include"../Manager/Game/UIManager.h"
#include"../Manager/Game/GameSetting.h"
#include "Select/SceneSelect.h"
#include "SceneTitle.h"

SceneTitle::SceneTitle(void)
{
	backImg_ = -1;
	logoImg_ = -1;
	pushAnyButtonImg_ = -1;
	postEffectScreen_ = -1;
	cnt_ = 0.0f;
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

	//UIの生成
	UIManager::CreateInstance(SingletonRegistry::DESTROY_TIMING::ALL_END);

	//ユーザー数
	int userNum = GameSetting::GetInstance().GetUserNum();

	//画面ごとのUI情報
	UIManager::GetInstance().CreateViewports(userNum, Application::SCREEN_SIZE_X, Application::SCREEN_SIZE_Y);

	//画像の読み込み
	backImg_ = res.Load(ResourceManager::SRC::TITLE_BACK).handleId_;
	logoImg_ = res.Load(ResourceManager::SRC::TITLE_LOGO).handleId_;
	pushAnyButtonImg_ = res.Load(ResourceManager::SRC::PUSH_ANY_BUTTON).handleId_;

	//サウンドの追加
	int id = res.Load(ResourceManager::SRC::ENTER_SE).handleId_;
	snd.Add(SoundManager::SOUND_NAME::ENTER, id, SoundManager::TYPE::SE, 80);

	//id = res.Load(ResourceManager::SRC::TITLE_SE).handleId_;
	//snd.Add(SoundManager::SOUND_NAME::TITLE_SE, id, SoundManager::TYPE::SE, 80);
	//snd.Play(SoundManager::SOUND_NAME::TITLE_SE, SoundManager::PLAYTYPE::LOOP);

	//UIのシェーディング
	uiMaterial_ = std::make_unique<PixelMaterial>(L"TitleUi.cso", 1);
	uiMaterial_->AddConstBuf({ cnt_, 0.0f, 0.0f, 0.0f });
	uiMaterial_->AddTextureBuf(pushAnyButtonImg_);
	uiRenderer_ = std::make_unique<PixelRenderer>(*uiMaterial_);
	uiRenderer_->SetPos({ PUSH_POS_X - PUSH_SIZE_X / 2, PUSH_POS_Y });
	uiRenderer_->SetSize({ PUSH_SIZE_X, PUSH_SIZE_Y });
	uiRenderer_->MakeSquereVertex();
}

void SceneTitle::Init(void)
{
	//分割なし
	SceneManager::GetInstance().SetIsSplitMode(false);

	//プレイヤー人数リセット
	GameSetting::GetInstance().ResetPlayerNum();

	//カウンタ
	cnt_ = 0.0f;
}

void SceneTitle::Update(void)
{
	//インスタンス
	auto& snd = SoundManager::GetInstance();
	auto& key = KeyConfig::GetInstance();
	auto& scn = SceneManager::GetInstance();

	//シーン遷移デバッグ
	if (key.IsTrgDownAny())
	{
		//決定音
		snd.Play(SoundManager::SOUND_NAME::ENTER, SoundManager::PLAYTYPE::BACK);

		//シーンの削除
		scn.ChangeScene(SceneManager::SCENE_ID::SELECT,true, true);
		return;
	}

	//カウンタ
	cnt_ += scn.GetDeltaTime();
}

void SceneTitle::Draw(const Camera& _camera)
{
#ifdef _DEBUG

	//デバッグ描画
	DebugDraw();

#endif // _DEBUG

	//背景
	DrawExtendGraph(0, 0, Application::SCREEN_SIZE_X, Application::SCREEN_SIZE_Y, backImg_, true);

	//ロゴ
	DrawExtendGraph(LOGO_POS_X_1, LOGO_POS_Y_1, LOGO_POS_X_2, LOGO_POS_Y_2, logoImg_, true);

	//プッシュボタン
	uiMaterial_->SetConstBuf(0, { cnt_, 0.0f, 0.0f, 0.0f });
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);
	uiRenderer_->Draw();
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);

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
