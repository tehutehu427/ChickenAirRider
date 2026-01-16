#pragma once
#include <functional>
#include "../SceneBase.h"

class SceneSelect : public SceneBase
{
public:

	//コンストラクタ
	SceneSelect(void);
	//デストラクタ
	~SceneSelect(void)override;

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

	//選択肢
	enum class SELECT_TYPE
	{
		HOME = -1,		//初期
		GAME_START,		//ゲーム開始
		OPTION,			//オプション
		TITLE,			//タイトル
		GAME_END,		//ゲーム終了
		MAX,
	};

	//プレイヤー人数選択
	enum class PLAYER_NUM_SELECT
	{
		USER,	//ユーザー
		NPC,	//NPC
		MAX
	};

	//選択肢の位置
	static constexpr int SELECT_POS_X = 200;
	static constexpr int SELECT_POS_Y = 200;
	static constexpr int SELECT_LOCAL_POS = 64;

	//プレイヤー人数の位置
	static constexpr int PLAYER_NUM_POS_X= 300;
	static constexpr int PLAYER_NUM_POS_Y = 300;
	static constexpr int PLAYER_NUM_LOCAL_POS = 300;

	//背景
	int backImg_;

	//選択肢
	SELECT_TYPE selectType_;
	SELECT_TYPE nowSelectType_;
	int selectTypeNum_;

	//プレイヤー人数
	PLAYER_NUM_SELECT playerNumSelect_;
	std::unordered_map<PLAYER_NUM_SELECT, int> playerNum_;

	//関数
	std::unordered_map<SELECT_TYPE, std::function<void(void)>> update_;
	std::unordered_map<SELECT_TYPE, std::function<void(void)>> draw_;

	//デバッグ描画
	void DebugDraw(void)override;

	//選択肢ごとの更新
	void UpdateHome(void);
	void UpdateGameStart(void);
	void UpdateOption(void);
	void UpdateTitle(void);
	void UpdateGameEnd(void);

	//選択肢ごとの描画
	void DrawHome(void);
	void DrawGameStart(void);
	void DrawOption(void);
	void DrawTitle(void);
	void DrawGameEnd(void);
};

