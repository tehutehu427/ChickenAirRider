#pragma once
#include "../Common/Singleton.h"

class Timer;

class GlobalUIManager : public Singleton<GlobalUIManager>
{
	//継承元のコンストラクタ等にアクセスするため
	friend class Singleton<GlobalUIManager>;

public:

	//描画種類
	enum class DRAW_TYPE
	{
		TIMER,				//タイマー
		LAST_COUNT_DOWN,	//最後のカウントダウン
		FINISH,				//終了表示
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

	//タイマー
	std::unique_ptr<Timer> timer_;					

	//フィニッシュUI
	int finishImg_;

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
};

