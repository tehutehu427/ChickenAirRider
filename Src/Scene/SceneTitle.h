#pragma once
#include "../Application.h"
#include "SceneBase.h"

class PixelMaterial;
class PixelRenderer;

class SceneTitle : public SceneBase
{
public:

	//コンストラクタ
	SceneTitle(void);
	//デストラクタ
	~SceneTitle(void)override;

	//読み込み
	void Load(void)override;
	//初期化
	void Init(void)override;
	//更新
	void Update(void)override;
	//描画
	void Draw(const Camera& _camera)override;
	//解放
	void Release(void)override;

private:

	//タイトルロゴ位置
	static constexpr float LOGO_POS_X_1 = Application::SCREEN_HALF_X - 350.0f;
	static constexpr float LOGO_POS_X_2 = Application::SCREEN_HALF_X + 350.0f;
	static constexpr float LOGO_POS_Y_1 = Application::SCREEN_HALF_Y - 250.0f;
	static constexpr float LOGO_POS_Y_2 = Application::SCREEN_HALF_Y + 50.0f;

	//プッシュボタンUIの位置
	static constexpr int PUSH_POS_X = Application::SCREEN_HALF_X;
	static constexpr int PUSH_POS_Y = Application::SCREEN_HALF_Y + Application::SCREEN_HALF_Y / 2;
	static constexpr int PUSH_SIZE_X = 600;
	static constexpr int PUSH_SIZE_Y = 80;

	//タイトル背景
	int backImg_;

	//タイトルロゴ
	int logoImg_;

	//プッシュボタンUI画像
	int pushAnyButtonImg_;

	int postEffectScreen_;
	std::unique_ptr<PixelMaterial> material_;
	std::unique_ptr<PixelRenderer> renderer_;

	//UI用
	std::unique_ptr<PixelMaterial> uiMaterial_;
	std::unique_ptr<PixelRenderer> uiRenderer_;

	//カウンタ
	float cnt_;

	//デバッグ描画
	void DebugDraw(void)override;
};

