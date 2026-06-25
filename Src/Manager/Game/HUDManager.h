#pragma once
#include<array>
#include "../Common/Singleton.h"
#include "../Common/Vector2F.h"
#include "../../Renderer/PixelRenderer.h"
#include "GameSetting.h"

class PixelMaterial;
class Player;

class HUDManager : public Singleton<HUDManager>
{
	//継承元のコンストラクタ等にアクセスするため
	friend class Singleton<HUDManager>;

public:

	//UI情報
	enum class HUD_TYPE
	{
		CHARGE_GAUGE,	//チャージゲージ
		HEALTH,			//体力
		PARAMETER,		//パラメーター
		MAX
	};

	//初期化
	void LoadOutSide(void)override;

	//初期化
	void Init(void)override;

	//更新
	void Update(void);

	//描画
	void Draw(void);

	//プレイヤーの設定
	void SetPlayer(const Player* _player, const int _index);

	//プレイヤーの表示設定
	void SetVisible(const int _index, const HUD_TYPE _type, const bool _visible);

private:

	//描画用関数ポインタ
	using DrawFunc = void(HUDManager::*)(const int);

	//描画情報
	struct HUDData
	{
		DrawFunc func;	//描画処理
		bool visible;	//描画判定
	};

	//プレイヤーUI情報
	struct PlayerHUD
	{
		const Player* player = nullptr;										//プレイヤー
		std::array<HUDData, static_cast<size_t>(HUD_TYPE::MAX)> hudData;	//描画情報
	};

	//描画パラメーターの紐付け
	struct ParamInfo
	{
		//値
		int value = -1;
		
		//アイコン画像
		int icon = -1;

		//色
		int color = 0;
	};
	
	//パラメーター
	static constexpr int PARAM_BOX_SIZE = 40;						//箱サイズ
	static constexpr int PARAM_BOX_INTERVAL = 20;					//箱の間隔
	static constexpr int POWER_UP_INTERVAL = 60;					//パワーアップ画像の間隔
	static constexpr int PARAM_NUMBER_LOCAL_POS = 100;				//パラメーター確認用数字の間隔
	static constexpr int NUMBER_INTERVAL = 35;						//数字間の間
	static constexpr Vector2F PARAM_BOX_START_POS = { 0.15f,0.1f };	//箱のUV座標

	//体力
	static constexpr int FRAME_SIZE = 2;
	static constexpr int HEALTH_BAR_WIDTH = 30;
	static constexpr int HEALTH_BAR_HEIGHT = 150;

	//UIの位置(Normalize)
	static constexpr Vector2F HEALTH_POS = { 0.95f,0.6f };
	static constexpr Vector2F CHARGE_POS = { 0.85f,0.8f };

	//ゲージの大きさ
	static constexpr float GAUGE_SIZE = 512.0f;
	static constexpr float GAUGE_SIZE_MULTI = 0.8f;

	//ゲージの位置補正
	static constexpr int GAUGE_LOCAL_POS = 5;
	static constexpr int NUMBER_LOCAL_POS = 30;

	//コンストラクタ
	HUDManager(void);

	//デストラクタ
	~HUDManager(void)override;

	//ゲージ画像
	int gaugeImg_;
	int gaugeMaskImg_;
	int* numImgs_;

	//パワーアップ画像
	int maxSpeedImg_;
	int acceleImg_;
	int turnImg_;
	int chargeImg_;
	int flightImg_;
	int weightImg_;
	int attackImg_;
	int defenceImg_;
	int maxHealthImg_;

	//シェーダー
	std::unique_ptr<PixelMaterial> material_;
	PixelRenderer renderer_;
	float gaugeCnt_;

	//プレイヤー参照
	std::array<PlayerHUD, static_cast<int>(GameSetting::PLAYER_MAX_NUM)> playerHUD_;

	//描画
	void DrawPlayerHUD(const int _playerIndex);

	//各描画
	void DrawChargeGauge(const int _playerIndex);
	void DrawHealth(const int _playerIndex);
	void DrawParam(const int _playerIndex);
};

