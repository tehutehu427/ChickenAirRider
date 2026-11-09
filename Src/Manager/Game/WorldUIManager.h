#pragma once
#include<array>
#include "../../Common/Singleton.h"
#include "GameSetting.h"

class Player;

class WorldUIManager : public Singleton<WorldUIManager>
{
	//継承元のコンストラクタ等にアクセスするため
	friend class Singleton<WorldUIManager>;

public:

	//UI情報
	enum class WORLD_UI_TYPE
	{
		ITEM_BOX,	//アイテムボックス
		MACHINE,	//機体
		MAX
	};

	//初期化
	void Init(void)override;

	//更新
	void Update(void);

	//描画
	void Draw(const int _playerIndex);

	//プレイヤーの設定
	void SetPlayer(const Player* _player, const int _index);

	//プレイヤーの表示設定
	void SetVisible(const int _index, const WORLD_UI_TYPE _type, const bool _visible);

private:

	//描画用関数ポインタ
	using DrawFunc = void(WorldUIManager::*)(const int);

	//描画情報
	struct HUDData
	{
		//描画処理
		DrawFunc func = nullptr;

		//描画判定
		bool visible = false;
	};

	//プレイヤーUI情報
	struct PlayerWorldUI
	{
		//プレイヤー
		const Player* player = nullptr;

		//描画情報
		std::array<HUDData, static_cast<size_t>(WORLD_UI_TYPE::MAX)> hudData;
	};

	//括弧のサイズ
	static constexpr float ATTENTION_SIZE = 3.0f;

	//注目括弧の表示距離
	static constexpr float ITEM_BOX_MAX_DISTANCE = 10000.0f;
	static constexpr float MACHINE_MAX_DISTANCE = 10000.0f;

	//プレイヤー参照
	std::array<PlayerWorldUI, static_cast<int>(GameSetting::PLAYER_MAX_NUM)> playerWorldUi_;

	//注目用括弧画像
	int attentionImg_;

	//コンストラクタ
	WorldUIManager(void);
	
	//デストラクタ
	~WorldUIManager(void)override;

	//読み込み
	void LoadOutSide(void)override;

	//描画
	void DrawWorldUI(const int _playerIndex);

	//描画
	void DrawItemBox(const int _playerIndex);
	void DrawMachine(const int _playerIndex);
};