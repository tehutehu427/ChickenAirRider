#pragma once
#include<array>
#include "../Common/Singleton.h"
#include "../../Renderer/PixelRenderer.h"

class PixelMaterial;
class Timer;

class GlobalUIManager : public Singleton<GlobalUIManager>
{
	//継承元のコンストラクタ等にアクセスするため
	friend class Singleton<GlobalUIManager>;

public:

	//描画種類
	enum class DRAW_TYPE
	{
		TIMER,		//タイマー
		COUNT_DOWN,	//カウントダウン
		FINISH,		//終了表示
		EVENT,		//イベント表示
		MAX
	};

	//初期化
	void LoadOutSide(void)override;

	//初期化
	void Init(void)override;

	//更新
	void Update(void);

	//描画
	void Draw(void);

	//表示設定
	void SetVisible(const DRAW_TYPE _type, const bool _visible);

	//全体タイマーの取得
	Timer& GetTimer(void) { return *timer_; }

	//イベントUIの設定
	void SetEventUI(const int _imgId) { eventImg_ = _imgId; };

private:

	//描画用関数ポインタ
	using DrawFunc = void(GlobalUIManager::*)(void);

	//描画情報
	struct DrawData
	{
		DrawFunc func;	//描画
		bool visible;	//表示の有無
	};

	//フィニッシュUIの拡大率
	static constexpr float FINISH_UI_SCALE = 1.0f;
	
	//枠の高さ
	static constexpr int FINISH_FRAME_HEIGHT = 100;
	
	//イベント
	static constexpr float EVENT_UI_SCALE = 0.5f;
	static constexpr int EVENT_LOCAL_POS_Y = -50;
	static constexpr int EVENT_FRAME_HEIGHT = 30;

	//タイマー
	std::unique_ptr<Timer> timer_;					

	//フィニッシュUI
	int finishImg_;

	//カウントダウン
	int* countDownImg_;
	std::unique_ptr<PixelMaterial> countDownMaterial_;

	//イベント
	int eventImg_;
	
	//レンダラー
	PixelRenderer renderer_;

	//描画
	std::array<DrawData,static_cast<int>(DRAW_TYPE::MAX)> draws_;

	//コンストラクタ
	GlobalUIManager(void);

	//デストラクタ
	~GlobalUIManager(void)override;

	//描画
	void DrawTimer(void);
	void DrawCountDown(void);
	void DrawFinish(void);
	void DrawEvent(void);
};

