#pragma once
#include<memory>
#include<unordered_map>
#include<functional>
#include "GameBase.h"

class Timer;

class GameCheck : public GameBase
{
public:

	//コンストラクタ
	GameCheck(SceneGame& _parent);

	//デストラクタ
	~GameCheck(void)override;

	//初期化
	void Init(void)override;

	//更新
	void Update(void)override;

	//描画
	void Draw(const Camera& _camera)override;

	//解放
	void Release(void)override;

private:

	//最終ゲーム確認時間
	static constexpr int LAST_GAME_CHECK_TIME = 5;

	//確認項目
	enum class CHECK_STATE
	{
		NONE = -1,
		PLAYER_PARAM,	//プレイヤーのステータス
		LAST_GAME,		//最後のゲーム
	};

	//確認項目
	CHECK_STATE state_;

	//更新
	std::unordered_map<CHECK_STATE, std::function<void(void)>> update_;

	//描画
	std::unordered_map<CHECK_STATE, std::function<void(void)>> draw_;

	//タイマー
	std::unique_ptr<Timer> timer_;

	//最終ゲーム画像
	std::unordered_map<int, int>lastGameImage_;
	std::unordered_map<int, int>lastGameTitle_;

	//デバッグ描画
	void DebugDraw(void)override;

	//更新
	void UpdatePlayerParam(void);
	void UpdateLastGame(void);

	//描画
	void DrawPlayerParam(void);
	void DrawLastGame(void);
};

