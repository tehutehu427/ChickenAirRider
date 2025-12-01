#pragma once
#include "../Common/Singleton.h"

class GameSetting : public Singleton<GameSetting>
{
	//継承元のコンストラクタ等にアクセスするため
	friend class Singleton<GameSetting>;

public:

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

private:

	//ユーザー数
	int userNum_;

	//NPC数
	int npcNum_;

	//コンストラクタ
	GameSetting(void);

	//デストラクタ
	~GameSetting(void)override;
};

