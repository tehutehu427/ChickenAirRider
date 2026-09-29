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
		MAX
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
	void Draw(const Camera& _camera)override;
	//解放
	void Release(void)override;

	//ゲームの状態の取得
	const GAME_STATE GetGameState(void)const { return gameState_; }

	//ゲームの状態の設定
	void ChangeGameState(const GAME_STATE _gameState);

	//最終ゲームの取得
	const LAST_GAME_TYPE GetLastGameType(void)const { return lastGameType_; };

private:

	//状態ごとのシーン
	GAME_STATE gameState_;
	std::unique_ptr<GameBase> game_;

	//最終ゲーム
	LAST_GAME_TYPE lastGameType_;

	//ゲーム生成関数ポインタ
	using CreateGameFunc = std::unique_ptr<GameBase>(SceneGame::*)(void);

	//ゲーム生成
	std::array<CreateGameFunc, static_cast<int>(GAME_STATE::MAX)> createGame_;

	//最終ゲーム生成
	std::array<CreateGameFunc, static_cast<int>(LAST_GAME_TYPE::MAX)> createLastGame_;
	
	//ソロでもできるかのチェック
	std::array<bool, static_cast<int>(LAST_GAME_TYPE::MAX)> soloLastGameJudge_;

	//最終ゲームを再構築
	void ResetLastGame(void);

	//ゲーム生成関数
	std::unique_ptr<GameBase> CreateGameMain(void);
	std::unique_ptr<GameBase> CreateGameCheck(void);

	//最終ゲーム生成関数
	std::unique_ptr<GameBase> CreateLastGameDeathMatch(void);
	std::unique_ptr<GameBase> CreateLastGameAirGlider(void);
};

