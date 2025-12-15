#pragma once
#include <memory>
#include <map>
#include <string>
#include "../Common/Singleton.h"
#include "Resource.h"

class ResourceManager : public Singleton<ResourceManager>
{
	//継承元のコンストラクタ等にアクセスするため
	friend class Singleton<ResourceManager>;

public:

	//数字分割画像用
	static constexpr int NUMBER_SIZE = 50;	//一画像サイズ
	static constexpr int NUMBER_NUM_X = 5;	//X分割数
	static constexpr int NUMBER_NUM_Y = 2;	//Y分割数

	// リソース名
	enum class SRC
	{
		NONE,

		//キャラクターモデル
		CHICKEN,			//チキンモデル
		
		//マシンモデル
		WAKABA,				//若葉モデル

		//ステージオブジェクトモデル
		STAGE,				//ステージモデル

		GLASS,				//草地モデル
		BUILDING,			//ビルモデル
		TREE,				//木モデル

		//アイテムモデル
		ITEM_BOX,			//アイテムボックス
		BOX_CRACK,			//箱のひび

		//アイテム画像
		MAX_SPEED,			//最高速
		ACCELERATION,		//加速
		TURNING,			//旋回
		CHARGE,				//チャージ
		FLIGHT,				//飛行
		WEIGHT,				//重さ
		ATTACK,				//攻撃
		DEFENCE,			//防御
		MAX_HEALTH,			//最大体力

		//スカイドーム
		SKY_DOME,			//スカイドーム

		//タイトル、セレクト
		TITLE_BACK,			//タイトル背景
		TITLE_LOGO,			//タイトルロゴ

		//UI
		CHARGE_GAUGE,			//チャージゲージ
		CHARGE_GAUGE_MASK,		//チャージゲージマスク
		SPEED_GAUGE,			//速度ゲージ
		NUMBER,					//数字
		COLON,					//コロン

		//サウンド

		//BGM
		SELECT_BGM,		//セレクトBGM
		MAIN_GAME_BGM,	//メインゲームBGM
		LAST_GAME_BGM,	//最終ミニゲームBGM
		RESULT_BGM,		//リザルトBGM

		//SE
		ENTER_SE,		//決定音
		SELECT_SE,		//選択音
		CANCEL_SE,		//キャンセル音

		ENGINE,			//エンジン音
		CHARGE_SE,		//チャージ音
		CHARGE_MAX_SE,	//チャージ完了SE
	};

	// 初期化
	void Init(void)override;

	// 解放(シーン切替時に一旦解放)
	void Release(void);

	// リソースの完全破棄
	void Destroy(void)override;

	// リソースのロード
	const Resource& Load(SRC src);

	// リソースの複製ロード(モデル用)
	int LoadModelDuplicate(SRC src);

protected:

	// デフォルトコンストラクタをprivateにして、
	// 外部から生成できない様にする
	ResourceManager(void);
	~ResourceManager(void)override;

private:

	// リソース管理の対象
	std::map<SRC, std::unique_ptr<Resource>> resourcesMap_;

	// 読み込み済みリソース
	std::map<SRC, Resource&> loadedMap_;

	Resource dummy_;

	// 内部ロード
	Resource& _Load(SRC src);

};
