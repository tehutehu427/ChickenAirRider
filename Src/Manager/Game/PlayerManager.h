#pragma once
#include<vector>
#include<memory>
#include"../../Common/Singleton.h"

class Player;

class PlayerManager : public Singleton<PlayerManager>
{
	//継承元のコンストラクタ等にアクセスするため
	friend class Singleton<PlayerManager>;

public:

	//初期化
	void Init(void)override;

	//更新
	void Update(void);

	//描画
	void Draw(void);

	//解放
	void Destroy(void);

	//プレイヤーの取得
	const Player* GetPlayer(const int _playerIndex) { return players_[_playerIndex].get(); }

	//プレイヤーの数の取得
	const int GetPlayerSize(void) { return static_cast<int>(players_.size()); }

	//プレイヤーの状態の初期化
	void ResetPlayer(const VECTOR _pos = {0.0f,0.0f,0.0f});

	//プレイヤーの降車判定
	void PlayerCanGetOff(const bool _getOff);

	//順位の取得
	const std::unordered_map<int, int> GetRanks(void)const { return ranks_; }

	//順位の設定
	void SetRanks(const std::unordered_map<int, int> _ranks) { ranks_ = _ranks; }

private:

	//プレイヤー
	std::vector<std::unique_ptr<Player>> players_;

	//順位
	std::unordered_map<int, int>ranks_;

	//コンストラクタ
	PlayerManager(void);

	//デストラクタ
	~PlayerManager(void)override;

	/// @brief プレイヤーの生成
	/// @param _playerIndex プレイヤー番号
	void CreateUserPlayer(const int _playerIndex);
	void CreateNpcPlayer(const int _playerIndex);
};

