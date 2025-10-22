#pragma once
#include "SceneBase.h"

class Timer;
class SkyDome;

class SceneGame : public SceneBase
{
public:

	//コンストラクタ
	SceneGame(void);
	//デストラクタ
	~SceneGame(void)override;

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

	//デバッグ描画
	void DebugDraw(void)override;

	//タイマー
	std::unique_ptr<Timer> timer_;

	//スカイドーム
	std::unique_ptr<SkyDome> sky_;
};

