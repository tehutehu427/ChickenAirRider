#pragma once
#include "GameBase.h"

class Timer;
class SkyDome;

class GameMain : public GameBase
{
public:

	//コンストラクタ
	GameMain(SceneGame& _parent);
	
	//デストラクタ
	~GameMain(void)override;

	//初期化
	void Init(void)override;

	//更新
	void Update(void)override;

	//描画
	void Draw(void)override;

	//解放
	void Release(void)override;

private:

	//デバッグ描画
	void DebugDraw(void)override;

	//タイマー
	std::unique_ptr<Timer> timer_;

	//スカイドーム
	std::unique_ptr<SkyDome> sky_;
};

