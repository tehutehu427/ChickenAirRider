#pragma once
#include<unordered_map>
#include<functional>
#include "../SceneBase.h"

class SceneOption : public SceneBase
{
public:

	//コンストラクタ
	SceneOption(void);	
	//デストラクタ
	~SceneOption(void)override;

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

private:

	//設定
	enum class OPTION_TYPE
	{
		HOME = -1,	//基本
		TIME,		//時間制限
		SCREEN,		//スクリーンサイズ
		KEY_TYPE,	//ボタンタイプ
		END,		//終了
		MAX,
	};

	//選択肢の位置
	static constexpr int SELECT_POS_X = 200;
	static constexpr int SELECT_POS_Y = 200;
	static constexpr int SELECT_LOCAL_POS_X = 300;
	static constexpr int SELECT_LOCAL_POS_Y = 64;

	//時間制限の制限
	static constexpr int TIME_LIMIT_MIN = 10;	
	static constexpr int TIME_LIMIT_MAX = 300;	

	//時間制限の倍率
	static constexpr int TIME_SEC_MULTI = 10;
	static constexpr int TIME_MIN_MULTI = 60;

	//キーの保持時間
	static constexpr float HOLD_TIME = 0.3f;

	//背景
	int backImg_;

	//設定
	OPTION_TYPE type_;
	OPTION_TYPE nowType_;
	int selectNum_;

	//変更前設定
	int oldTimeLimit_;
	bool oldScreenSize_;
	int oldKeyType_;

	//変更後
	int modiTimeLimit_;
	bool modiScreenSize_;
	int modiKeyType_;

	//時間制限の変更倍率
	int timeMulti_;

	//更新と描画
	std::unordered_map<OPTION_TYPE, std::function<void(void)>> update_;
	std::unordered_map<OPTION_TYPE, std::function<void(void)>> draw_;

	//更新
	void UpdateHome(void);
	void UpdateTime(void);
	void UpdateScreen(void);
	void UpdateKeyType(void);
	void UpdateEnd(void);

	//描画
	void DrawHome(void);
	void DrawTime(void);
	void DrawScreen(void);
	void DrawKeyType(void);
	void DrawEnd(void);
};