#pragma once
#include<vector>
#include<memory>

class Player;

class PlayerManager
{
public:

	//インスタンス生成
	static void CreateInstance(void);

	//インスタンス取得
	static PlayerManager& GetInstance(void) { return *instance_; }

	//初期化
	void Init(void);

	//更新
	void Update(void);

	//描画
	void Draw(void);

	//解放
	void Destroy(void);

	/// @brief プレイヤーの生成
	/// @param _userNum ユーザーの数
	/// @param _npcNum NPCの数
	void CreatePlayer(const int _userNum, const int _npcNum);

	//ユーザー数の取得
	const int GetUserNum(void) { return userNum_; }

	//ユーザー数の設定
	void SetUserNum(const int _userNum) { userNum_ = _userNum; }

	//NPC数の取得
	const int GetNpcNum(void) { return npcNum_; }

	//NPC数の設定
	void SetNpcNum(const int _npcNum) { npcNum_ = _npcNum; }

	//全体プレイヤーの数の取得
	const int GetPlayerNum(void) { return userNum_ + npcNum_; }

	//プレイヤーの取得
	const Player& GetPlayer(int _playerIndex) { return *players_[_playerIndex]; }

private:

	//静的インスタンス
	static PlayerManager* instance_;

	//プレイヤー
	std::vector<std::unique_ptr<Player>> players_;

	//ユーザー数
	int userNum_;

	//NPC数
	int npcNum_;

	//コンストラクタ
	PlayerManager(void);

	//コピー禁止
	PlayerManager(const PlayerManager& _copy) = delete;
	PlayerManager& operator= (const PlayerManager& _copy) = delete;

	//デストラクタ
	~PlayerManager(void);
};

