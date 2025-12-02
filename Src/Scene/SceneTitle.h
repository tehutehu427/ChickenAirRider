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
	void Draw(void)override;
	//解放
	void Release(void)override;

private:

	//タイトルロゴ位置
	static constexpr float LOGO_POS_X_1 = Application::SCREEN_HALF_X - 350.0f;
	static constexpr float LOGO_POS_X_2 = Application::SCREEN_HALF_X + 350.0f;
	static constexpr float LOGO_POS_Y_1 = Application::SCREEN_HALF_Y - 250.0f;
	static constexpr float LOGO_POS_Y_2 = Application::SCREEN_HALF_Y + 50.0f;

	//タイトル背景
	int backImg_;

	//タイトルロゴ
	int logoImg_;

	//std::unique_ptr<PixelMaterial> material_;
	//std::unique_ptr<PixelRenderer> material_;

	//デバッグ描画
	void DebugDraw(void)override;
};

