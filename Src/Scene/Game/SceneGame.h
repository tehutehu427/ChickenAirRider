#pragma once
#include "../SceneBase.h"

class GameBase;

class SceneGame : public SceneBase
{
public:

	//ゲームの状態
	enum class GAME_STATE
	{
		MAIN,
		CHECK,
		LAST,
	};

	//最終ゲームの種類
	enum class LAST_GAME_TYPE
	{
		DEATH_MATCH,	//デスマッチ
		AIR_GLIDER,		//エアグライダー
		MAX
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
	void ChangeGameState(const GAME_STATE _gameState);

private:

	//状態ごとのシーン
	GAME_STATE gameState_;
	std::unique_ptr<GameBase> game_;

	//ゲーム生成
	std::unordered_map<GAME_STATE, std::function<std::unique_ptr<GameBase>(void)>> createGame_;

	//最終ゲーム生成
	std::unordered_map<LAST_GAME_TYPE, std::function<std::unique_ptr<GameBase>(void)>> createLastGame_;

	//最後のミニゲームを再構築
	void ResetLastGame(void);
};

