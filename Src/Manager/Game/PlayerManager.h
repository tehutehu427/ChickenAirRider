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

	//プレイヤーの取得
	const Player& GetPlayer(const int _playerIndex) { return *players_[_playerIndex]; }

	//プレイヤーの状態の初期化
	void ResetPlayer(const VECTOR _pos = {0.0f,0.0f,0.0f});

	//プレイヤーの降車判定
	void PlayerCanGetOff(const bool _getOff);

private:

	//静的インスタンス
	static PlayerManager* instance_;

	//プレイヤー
	std::vector<std::unique_ptr<Player>> players_;

	//コンストラクタ
	PlayerManager(void);

	//コピー禁止
	PlayerManager(const PlayerManager& _copy) = delete;
	PlayerManager& operator= (const PlayerManager& _copy) = delete;

	//デストラクタ
	~PlayerManager(void);

	/// @brief プレイヤーの生成
	/// @param _playerIndex プレイヤー番号
	void CreateUserPlayer(const int _playerIndex);
	void CreateNpcPlayer(const int _playerIndex);
};

