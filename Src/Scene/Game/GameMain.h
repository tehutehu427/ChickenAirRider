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
	void Draw(const Camera& _camera)override;

	//解放
	void Release(void)override;

private:

	//状態
	enum class STATE
	{
		GAME,	//ゲーム中
		FIN,	//終了
	};

	//終了表示時間
	static constexpr float FINISH_TIME = 2.0f;

	//カウントダウン時間
	static constexpr float LAST_COUNT_DOWN = 5.0f;

	//カウンタ
	float cnt_;

	//状態
	STATE state_;

	//更新
	std::unordered_map<STATE, std::function<void(void)>> update_;

	//描画
	std::unordered_map<STATE, std::function<void(const Camera& _camera)>> draw_;

	//デバッグ描画
	void DebugDraw(void)override;

	//更新
	void UpdateGame(void);
	void UpdateFinish(void);

	//描画
	void DrawGame(const Camera& _camera);
	void DrawFinish(const Camera& _camera);
};

