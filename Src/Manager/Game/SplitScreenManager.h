#pragma once
#include<unordered_map>
#include<memory>
#include<array>
#include "../Common/Singleton.h"
#include "../Common/Vector2F.h"

class Camera;
class PixelMaterial;
class PixelRenderer;

class SplitScreenManager : public Singleton<SplitScreenManager>
{
	//継承元のコンストラクタ等にアクセスするため
	friend class Singleton<SplitScreenManager>;

public:

	//画面分割の最大数
	static constexpr int MAX_SPLIT_NUM = 4;

	//ビューポート
	struct Viewport
	{
		int x = 0, y = 0;			//ビューポート座標(左上)
		int w = 0, h = 0;			//ビューポートの大きさ
	};

	//正規化UI座標
	struct NormalizedPos
	{
		float x = 0.0f, y = 0.0f;		//UI座標(0.0～1.0)
	};

	//分割描画情報
	struct SplitViewInfo
	{
		int screen = -1;					//描画スクリーン
		std::weak_ptr<Camera> camera = {};	//カメラ
		Viewport viewPort = {};				//ビューポート
	};

	//初期化
	void LoadOutSide(void)override;

	//初期化
	void Init(void)override;

	//更新
	void Update(void);

	//描画
	void Draw(const int _playerIndex = 0);

	//描画するUIの追加
	void AddDraw(const DRAW_TYPE _type, const int _playerIndex);

	//描画するUIの削除
	void SubDraw(const DRAW_TYPE _type, const int _playerIndex);

	//ビューポートの生成
	void CreateViewports(const int _playerCnt, const int _screenW, const int _screenH);

	//ビューポートの取得
	const Viewport& GetViewport(const int _playerIndex);

private:

	//パラメーター
	static constexpr int PARAM_BOX_SIZE = 40;						//箱サイズ
	static constexpr int PARAM_BOX_INTERVAL = 20;					//箱の間隔
	static constexpr int POWER_UP_INTERVAL = 150;					//パワーアップ画像の間隔
	static constexpr int PARAM_NUMBER_LOCAL_POS = 100;				//パラメーター確認用数字の間隔
	static constexpr int NUMBER_INTERVAL = 30;						//数字間の間
	static constexpr Vector2F PARAM_BOX_START_POS = { 0.2f,0.1f };	//箱のUV座標

	//体力
	static constexpr int HEALTH_BOX_LOCAL_POS_X = 10;
	static constexpr int HEALTH_BOX_LOCAL_POS_Y = 0;
	static constexpr int HEALTH_BOX = 1;
	static constexpr int HEALTH_LOCAL = 3;
	static constexpr int HEALTH_BAR_HEIGHT = 100;

	//UIの位置(Normalize)
	static constexpr Vector2F HEALTH_POS = { 0.95f,0.6f };
	static constexpr Vector2F CHARGE_POS = { 0.85f,0.8f };

	//ゲージの大きさ
	static constexpr float GAUGE_SIZE = 512.0f;
	static constexpr float GAUGE_SIZE_MULTI = 0.8f;

	//ゲージの位置補正
	static constexpr int GAUGE_LOCAL_POS = 5;
	static constexpr int NUMBER_LOCAL_POS = 40;

	//描画情報
	std::array<SplitViewInfo, MAX_SPLIT_NUM> splitViews_;

	//描画
	std::unordered_map<DRAW_TYPE, std::function<void(int)>> drawList_;
	std::unordered_map<int, std::unordered_map<DRAW_TYPE, std::function<void(int)>>> drawView_;

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
	std::unique_ptr<PixelRenderer> renderer_;
	float gaugeCnt_;

	//コンストラクタ
	SplitScreenManager(void);

	//デストラクタ
	~SplitScreenManager(void)override;

	//描画
	void DrawChargeGauge(const int _playerIndex);
	void DrawHealth(const int _playerIndex);
	void DrawParam(const int _playerIndex);
};

