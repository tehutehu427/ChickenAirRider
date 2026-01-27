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
	};

	//初期化
	void LoadOutSide(void)override;

	//初期化
	void Init(void)override;

	//更新
	void Update(void);

	//描画
	void Draw(void);

	//描画するUIの追加
	void AddDraw(const DRAW_TYPE _type);

	//描画するUIの削除
	void SubDraw(const DRAW_TYPE _type);

	//全体タイマーの取得
	Timer& GetTimer(void) { return *timer_; }

private:

	//タイマー
	std::unique_ptr<Timer> timer_;					

	//描画
	std::unordered_map<DRAW_TYPE, std::function<void(void)>> drawList_;
	std::unordered_map<DRAW_TYPE, std::function<void(void)>> drawView_;

	//コンストラクタ
	GlobalUIManager(void);

	//デストラクタ
	~GlobalUIManager(void)override;

	//描画
	void DrawTimer(void);
	void DrawCountDown(void);
	void DrawFinish(void);
};

