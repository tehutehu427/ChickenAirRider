#pragma once
#include<array>
#include<queue>
#include<string>
#include "../../Common/Singleton.h"
#include"GameSetting.h"

class GuideManager : public Singleton<GuideManager>
{
	//継承元のコンストラクタ等にアクセスするため
	friend class Singleton<GuideManager>;

public:

	//説明の種類
	enum class GUIDE_TYPE
	{
		CHARGE,			//チャージ、ダッシュ
		SPIN,			//スピン
		ITEM_BOX,		//アイテム
		POWER_UP_ITEM,	//パワーアップアイテム
		BATTLE_ITEM,	//バトルアイテム
		GET_OFF,		//降車
		MACHINE,		//機体
		MAX
	};

	//初期化
	void Init(void)override;

	//更新
	void Update(void);

	//描画
	void Draw(void);

	//表示の有無のリセット
	void ResetIsDisplay(void);

	//ガイドの追加
	void Add(const GUIDE_TYPE& _type, const int _playerIndex);

	//ガイドの表示判定の設定
	void SetIsDisplay(const GUIDE_TYPE& _type, const int _playerIndex, const bool _isDisplay);

private:

	//ガイドテキストのサイズ
	static constexpr int TEXT_SIZE = 40;
	static constexpr int TEXT_THICK = 10;

	//表示時間
	static constexpr float DISPLAY_TIME = 3.0f;

	//表示用カウンタ
	std::array<float,GameSetting::PLAYER_MAX_NUM>cnt_;

	//表示情報
	std::array<std::wstring, static_cast<int>(GUIDE_TYPE::MAX)> text_;

	//描画
	std::array<std::queue<GUIDE_TYPE>,GameSetting::PLAYER_MAX_NUM> type_;

	//表示の有無
	std::array<std::array<bool, static_cast<int>(GUIDE_TYPE::MAX)>, GameSetting::PLAYER_MAX_NUM> isDisplay_;

	//コンストラクタ
	GuideManager(void);

	//デストラクタ
	~GuideManager(void)override;

	//外部読み込み
	void LoadOutSide(void)override;

	//ガイドの描画
	void DrawGuide(const int _playerIndex);
};