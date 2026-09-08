#pragma once
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
	static constexpr int LOGO_LOCAL_POS_X_1 = -350;
	static constexpr int LOGO_LOCAL_POS_X_2 = 350;
	static constexpr int LOGO_LOCAL_POS_Y_1 = -250;
	static constexpr int LOGO_LOCAL_POS_Y_2 = 50;

	//プッシュボタンUIの位置
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

