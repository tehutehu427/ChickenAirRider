#pragma once
#include<vector>
#include<memory>

class Player;

class PlayerManager
{
public:

	//プレイヤーの最大数
	static constexpr int PLAYER_MAX_NUM = 4;

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
	const Player& GetPlayer(int _playerIndex) { return *players_[_playerIndex]; }

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

