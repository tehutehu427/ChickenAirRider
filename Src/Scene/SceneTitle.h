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
	static constexpr float BLINKING_SPEED = 2.0f;

	//歯車の位置
	static constexpr int GEAR_LOCAL_POS_X = 500;
	static constexpr int GEAR_LOCAL_POS_Y = 250;

	//歯車の大きさ
	static constexpr float GEAR_SIZE = 2.0f;

	//歯車の速度の差異
	static constexpr float GEAR_SPEED_MULTI = 2.0f;

	//SEの音量
	static constexpr int SE_VOLUME = 80;

	//タイトル背景
	int backImg_;
	int backFrameImg_;
	int gearImg_;

	//タイトルロゴ
	int logoImg_;

	//プッシュボタンUI画像
	int pushAnyButtonImg_;

	//ポストエフェクト用スクリーン
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

