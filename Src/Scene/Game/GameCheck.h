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

	//文字背景
	static constexpr int TEXT_BOX_RANGE = 100;

	//最終ゲーム確認時間
	static constexpr int LAST_GAME_CHECK_TIME = 5;

	//最終ゲームの画像の拡大率
	static constexpr float LAST_GAME_IMAGE_SIZE = 1.5f;

	//確認項目
	enum class CHECK_STATE
	{
		PLAYER_PARAM,	//プレイヤーのステータス
		LAST_GAME,		//最後のゲーム
		MAX
	};

	//確認項目
	CHECK_STATE state_;

	//関数ポインタ
	using Func = void(GameCheck::*)(void);

	//更新
	std::array<Func, static_cast<int>(CHECK_STATE::MAX)> update_;

	//描画
	std::array<Func, static_cast<int>(CHECK_STATE::MAX)> draw_;

	//タイマー
	std::unique_ptr<Timer> timer_;

	//最終ゲーム画像
	std::array<int, static_cast<int>(SceneGame::LAST_GAME_TYPE::MAX)>lastGameImage_;
	std::array<int, static_cast<int>(SceneGame::LAST_GAME_TYPE::MAX)>lastGameTitle_;

	//デバッグ描画
	void DebugDraw(void)override;

	//更新
	void UpdatePlayerParam(void);
	void UpdateLastGame(void);

	//描画
	void DrawPlayerParam(void);
	void DrawLastGame(void);
};

