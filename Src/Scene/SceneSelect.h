#pragma once
#include <functional>
#include "SceneBase.h"

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
		GAME_START,		//ゲーム開始
		OPTION,			//オプション
		GAME_END,		//ゲーム終了
		MAX,
	};

	//選択肢の位置
	static constexpr int SELECT_POS_X = 200;
	static constexpr int SELECT_POS_Y = 200;
	static constexpr int SELECT_LOCAL_POS = 64;

	//背景
	int backImg_;

	//選択肢
	SELECT_TYPE selectType_;
	int selectTypeNum_;

	//関数
	std::unordered_map<SELECT_TYPE, std::function<void(void)>> selectEnter_;

	//デバッグ描画
	void DebugDraw(void)override;

	//選択肢の効果
	void SelectGameStart(void);
	void SelectOption(void);
	void SelectGameEnd(void);
};

