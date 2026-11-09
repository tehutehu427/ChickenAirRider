#pragma once
#include<array>
#include<queue>
#include<string>
#include "../../Common/Singleton.h"
#include "../../Common/Vector2.h"
#include"GameSetting.h"
#include"../../Renderer/PixelRenderer.h"

class PixelMaterial;

class GuideManager : public Singleton<GuideManager>
{
	//継承元のコンストラクタ等にアクセスするため
	friend class Singleton<GuideManager>;

public:

	//説明の種類
	enum class GUIDE_TYPE
	{
		//操作説明
		BASIC,			//基本説明
		CHARGE,			//チャージ、ダッシュ
		SPIN,			//スピン
		GET_OFF,		//降車

		//オブジェクト
		ITEM_BOX,		//アイテム
		POWER_UP_ITEM,	//パワーアップアイテム
		BATTLE_ITEM,	//バトルアイテム
		MACHINE,		//機体

		//ラストゲーム
		AIR_GLIDER,		//エアグライダー
		DEATH_MATCH,	//デスマッチ

		MAX
	};

	//初期化
	void Init(void)override;

	//更新
	void Update(void);

	//描画
	void Draw(void);

	//ガイドの要素のリセット
	void Reset(void);

	//ガイドの追加
	void Add(const GUIDE_TYPE& _type, const int _playerIndex);

	//完了した
	void Conpleted(const GUIDE_TYPE& _type, const int _playerIndex);

private:

	//ガイドの要素
	struct GuideData
	{
		//項目をクリアしたか
		bool isClear = false;

		//カウンタ
		float cnt = 0.0f;
	};

	//ガイドテキストのサイズ
	static constexpr int TEXT_SIZE = 25;
	static constexpr int TEXT_THICK = 5;

	//ボーダーサイズ
	static constexpr float SRC_BORDER_SIZE = 360.0f;
	static constexpr float DST_BORDER_SIZE = 60.0f;

	//説明の相対座標
	static constexpr Vector2 LOCAL_POS = { 30,30 };

	//表示時間
	static constexpr float DISPLAY_TIME = 3.0f;

	//表示情報
	std::array<std::wstring, static_cast<int>(GUIDE_TYPE::MAX)> text_;

	//表示する種類
	std::array<std::queue<GUIDE_TYPE>,GameSetting::PLAYER_MAX_NUM> types_;

	//ガイドの要素
	std::array<std::array<GuideData, static_cast<int>(GUIDE_TYPE::MAX)>, GameSetting::PLAYER_MAX_NUM> guideDatas_;

	//ピクセルシェーダー
	PixelRenderer renderer_;
	std::unique_ptr<PixelMaterial> dialogMaterial_;

	//ダイアログのサイズ
	Vector2 dialogSize_;

	//コンストラクタ
	GuideManager(void);

	//デストラクタ
	~GuideManager(void)override;

	//外部読み込み
	void LoadOutSide(void)override;

	//ガイドの描画
	void DrawGuide(const int _playerIndex);
};