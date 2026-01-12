#pragma once
#include "../Common/Singleton.h"

class GameSetting : public Singleton<GameSetting>
{
	//継承元のコンストラクタ等にアクセスするため
	friend class Singleton<GameSetting>;

public:

	//プレイヤーの最大数
	static constexpr int PLAYER_MAX_NUM = 4;

	//初期化
	void Init(void)override;

	//ユーザー数の取得
	const int GetUserNum(void)const { return userNum_; }

	//ユーザー数の設定
	void SetUserNum(const int _userNum) { userNum_ = _userNum; }

	//NPC数の取得
	const int GetNpcNum(void)const { return npcNum_; }

	//NPC数の設定
	void SetNpcNum(const int _npcNum) { npcNum_ = _npcNum; }

	//全体プレイヤーの数の取得
	const int GetPlayerNum(void)const { return userNum_ + npcNum_; }

	//プレイヤー数のリセット
	void ResetPlayerNum(void);

	//時間制限の取得
	const int GetTimeLimit(void)const { return timeLimit_; }

	//時間制限の設定
	void SetTimeLimit(const int _timeLimit){ timeLimit_ = _timeLimit; }

private:

	//ユーザー数
	int userNum_;

	//NPC数
	int npcNum_;

	//時間制限
	int timeLimit_;

	//コンストラクタ
	GameSetting(void);

	//デストラクタ
	~GameSetting(void)override;
};

