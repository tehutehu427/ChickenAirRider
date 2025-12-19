#pragma once
#include "../SceneBase.h"

class GameBase;

class SceneGame : public SceneBase
{
public:

	//ゲームの状態
	enum class GAME_STATE
	{
		NONE = -1,
		MAIN,
		CHECK,
		LAST,
	};

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

	//ゲームの状態の取得
	const GAME_STATE GetGameState(void)const { return gameState_; }

	//ゲームの状態の設定
	void SetGameState(const GAME_STATE _gameState) { gameState_ = _gameState; }

private:

	//状態ごとのシーン
	GAME_STATE gameState_;
	std::unordered_map<GAME_STATE, std::unique_ptr<GameBase>> game_;
};

