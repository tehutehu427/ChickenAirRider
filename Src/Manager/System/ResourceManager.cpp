#include"../pch.h"
#include "../../Application.h"
#include "Resource.h"
#include "ResourceManager.h"

void ResourceManager::Init(void)
{
	std::wstring PATH_IMG = Application::PATH_IMAGE;
	std::wstring PATH_MDL = Application::PATH_MODEL;
	std::wstring PATH_EFF = Application::PATH_EFFECT;
	std::wstring PATH_SOUND = Application::PATH_SOUND;

	std::unique_ptr<Resource> res;

#pragma region 画像

	//アイテム画像のパス
	std::wstring itemImage = PATH_IMG + L"Item/";

	//タイトル画像のパス
	std::wstring titleImage = PATH_IMG + L"Title/";

	//セレクト画像のパス
	std::wstring selectImage = PATH_IMG + L"Select/";

	//リザルト画像のパス
	std::wstring resultImage = PATH_IMG + L"Result/";

	//UI画像のパス
	std::wstring uiImage = PATH_IMG + L"UI/";

	//箱ひび画像
	res = std::make_unique<Resource>(Resource::TYPE::IMG, itemImage + L"BoxCrack.png");
	resourcesMap_.emplace(SRC::BOX_CRACK, std::move(res));

	//最高速画像
	res = std::make_unique<Resource>(Resource::TYPE::IMG, itemImage + L"MaxSpeed.png");
	resourcesMap_.emplace(SRC::MAX_SPEED, std::move(res));

	//加速画像
	res = std::make_unique<Resource>(Resource::TYPE::IMG, itemImage + L"Acceleration.png");
	resourcesMap_.emplace(SRC::ACCELERATION, std::move(res));
	
	//旋回画像
	res = std::make_unique<Resource>(Resource::TYPE::IMG, itemImage + L"Turning.png");
	resourcesMap_.emplace(SRC::TURNING, std::move(res));

	//チャージ画像
	res = std::make_unique<Resource>(Resource::TYPE::IMG, itemImage + L"Charge.png");
	resourcesMap_.emplace(SRC::CHARGE, std::move(res));

	//飛行画像
	res = std::make_unique<Resource>(Resource::TYPE::IMG, itemImage + L"Flight.png");
	resourcesMap_.emplace(SRC::FLIGHT, std::move(res));

	//重さ画像
	res = std::make_unique<Resource>(Resource::TYPE::IMG, itemImage + L"Weight.png");
	resourcesMap_.emplace(SRC::WEIGHT, std::move(res));

	//攻撃画像
	res = std::make_unique<Resource>(Resource::TYPE::IMG, itemImage + L"Attack.png");
	resourcesMap_.emplace(SRC::ATTACK, std::move(res));

	//防御画像
	res = std::make_unique<Resource>(Resource::TYPE::IMG, itemImage + L"Defence.png");
	resourcesMap_.emplace(SRC::DEFENCE, std::move(res));

	//最大体力画像
	res = std::make_unique<Resource>(Resource::TYPE::IMG, itemImage + L"MaxHealth.png");
	resourcesMap_.emplace(SRC::MAX_HEALTH, std::move(res));

	//キャノン画像
	res = std::make_unique<Resource>(Resource::TYPE::IMG, itemImage + L"Cannon.png");
	resourcesMap_.emplace(SRC::CANNON_IMAGE, std::move(res));
	
	//タイトル背景
	res = std::make_unique<Resource>(Resource::TYPE::IMG, titleImage + L"TitleBack.png");
	resourcesMap_.emplace(SRC::TITLE_BACK, std::move(res));

	//タイトル背景の枠
	res = std::make_unique<Resource>(Resource::TYPE::IMG, titleImage + L"TitleBackFrame.png");
	resourcesMap_.emplace(SRC::TITLE_BACK_FRAME, std::move(res));

	//タイトルの歯車
	res = std::make_unique<Resource>(Resource::TYPE::IMG, titleImage + L"Gear.png");
	resourcesMap_.emplace(SRC::TITLE_GEAR, std::move(res));

	//タイトルロゴ
	res = std::make_unique<Resource>(Resource::TYPE::IMG, titleImage + L"TitleLogo.png");
	resourcesMap_.emplace(SRC::TITLE_LOGO, std::move(res));

	//何かボタンを押してください画像
	res = std::make_unique<Resource>(Resource::TYPE::IMG, titleImage + L"PushAnyButton.png");
	resourcesMap_.emplace(SRC::PUSH_ANY_BUTTON, std::move(res));

	//セレクトメニューバー
	res = std::make_unique<Resource>(Resource::TYPE::IMG, selectImage + L"SelectMenu.png");
	resourcesMap_.emplace(SRC::MENU_BAR, std::move(res));

	//ゲームスタートテキスト
	res = std::make_unique<Resource>(Resource::TYPE::IMG, selectImage + L"GameStart.png");
	resourcesMap_.emplace(SRC::GAME_START_TEXT, std::move(res));
	
	//オプションテキスト
	res = std::make_unique<Resource>(Resource::TYPE::IMG, selectImage + L"Option.png");
	resourcesMap_.emplace(SRC::OPTION_TEXT, std::move(res));

	//タイトルへテキスト
	res = std::make_unique<Resource>(Resource::TYPE::IMG, selectImage + L"ForTitle.png");
	resourcesMap_.emplace(SRC::GO_TITLE_TEXT, std::move(res));

	//ゲーム終了テキスト
	res = std::make_unique<Resource>(Resource::TYPE::IMG, selectImage + L"EndGame.png");
	resourcesMap_.emplace(SRC::GAME_END_TEXT, std::move(res));

	//プレイヤー人数テキスト
	res = std::make_unique<Resource>(Resource::TYPE::IMG, selectImage + L"PlayerNumberText.png");
	resourcesMap_.emplace(SRC::PLAYER_NUM_TEXT, std::move(res));

	//制限時間テキスト
	res = std::make_unique<Resource>(Resource::TYPE::IMG, selectImage + L"TimeLimit.png");
	resourcesMap_.emplace(SRC::TIME_LIMIT_TEXT, std::move(res));

	//フルスクリーンテキスト
	res = std::make_unique<Resource>(Resource::TYPE::IMG, selectImage + L"FullScreen.png");
	resourcesMap_.emplace(SRC::FULL_SCREEN_TEXT, std::move(res));

	//オンテキスト
	res = std::make_unique<Resource>(Resource::TYPE::IMG, selectImage + L"On.png");
	resourcesMap_.emplace(SRC::ON_TEXT, std::move(res));

	//オフテキスト
	res = std::make_unique<Resource>(Resource::TYPE::IMG, selectImage + L"Off.png");
	resourcesMap_.emplace(SRC::OFF_TEXT, std::move(res));

	//キーコンフィグテキスト
	res = std::make_unique<Resource>(Resource::TYPE::IMG, selectImage + L"KeyConfig.png");
	resourcesMap_.emplace(SRC::KEY_CONFIG_TEXT, std::move(res));

	//終了テキスト
	res = std::make_unique<Resource>(Resource::TYPE::IMG, selectImage + L"End.png");
	resourcesMap_.emplace(SRC::END_TEXT, std::move(res));

	//Push画像
	res = std::make_unique<Resource>(Resource::TYPE::IMG, uiImage + L"Push.png");
	resourcesMap_.emplace(SRC::PUSH, std::move(res));

	//マウス
	res = std::make_unique<Resource>(Resource::TYPE::IMG, uiImage + L"Mouse.png");
	resourcesMap_.emplace(SRC::MOUSE, std::move(res));

	//マウス左クリック
	res = std::make_unique<Resource>(Resource::TYPE::IMG, uiImage + L"MouseLeft.png");
	resourcesMap_.emplace(SRC::MOUSE_LEFT, std::move(res));

	//マウス右クリック
	res = std::make_unique<Resource>(Resource::TYPE::IMG, uiImage + L"MouseRight.png");
	resourcesMap_.emplace(SRC::MOUSE_RIGHT, std::move(res));

	//マウス左右移動
	res = std::make_unique<Resource>(Resource::TYPE::IMG, uiImage + L"MouseMove.png");
	resourcesMap_.emplace(SRC::MOUSE_MOVE, std::move(res));

	//Aボタン
	res = std::make_unique<Resource>(Resource::TYPE::IMG, uiImage + L"AButton.png");
	resourcesMap_.emplace(SRC::A_BUTTON, std::move(res));

	//Bボタン
	res = std::make_unique<Resource>(Resource::TYPE::IMG, uiImage + L"BButton.png");
	resourcesMap_.emplace(SRC::B_BUTTON, std::move(res));

	//Xボタン
	res = std::make_unique<Resource>(Resource::TYPE::IMG, uiImage + L"XButton.png");
	resourcesMap_.emplace(SRC::X_BUTTON, std::move(res));

	//Yボタン
	res = std::make_unique<Resource>(Resource::TYPE::IMG, uiImage + L"YButton.png");
	resourcesMap_.emplace(SRC::Y_BUTTON, std::move(res));

	//UIのチャージゲージ画像
	res = std::make_unique<Resource>(Resource::TYPE::IMG, uiImage + L"ChargeGauge.png");
	resourcesMap_.emplace(SRC::CHARGE_GAUGE, std::move(res));

	//数字画像
	res = std::make_unique<Resource>(Resource::TYPE::IMGS, uiImage + L"Number.png", NUMBER_NUM_X, NUMBER_NUM_Y, NUMBER_SIZE, NUMBER_SIZE);
	resourcesMap_.emplace(SRC::NUMBER, std::move(res));
	
	//コロン画像
	res = std::make_unique<Resource>(Resource::TYPE::IMG, uiImage + L"Colon.png");
	resourcesMap_.emplace(SRC::COLON, std::move(res));

	//タイマーの枠画像
	res = std::make_unique<Resource>(Resource::TYPE::IMG, uiImage + L"TimerFrame.png");
	resourcesMap_.emplace(SRC::TIMER_FRAME, std::move(res));

	//終了用の数字画像
	res = std::make_unique<Resource>(Resource::TYPE::IMGS, uiImage + L"FinishNumber.png", NUMBER_NUM_X, NUMBER_NUM_Y, NUMBER_SIZE, NUMBER_SIZE);
	resourcesMap_.emplace(SRC::FINISH_NUMBER, std::move(res));
		
	//終了UI画像
	res = std::make_unique<Resource>(Resource::TYPE::IMG, uiImage + L"Finish.png");
	resourcesMap_.emplace(SRC::FINISH_UI, std::move(res));

	//デスマッチの確認画像
	res = std::make_unique<Resource>(Resource::TYPE::IMG, uiImage + L"DeathMatchExplan.png");
	resourcesMap_.emplace(SRC::DEATH_MATCH_CHECK_IMAGE, std::move(res));

	//デスマッチの確認見出し
	res = std::make_unique<Resource>(Resource::TYPE::IMG, uiImage + L"DeathMatchExplanText.png");
	resourcesMap_.emplace(SRC::DEATH_MATCH_CHECK_TITLE, std::move(res));

	//エアグライダーの確認画像
	res = std::make_unique<Resource>(Resource::TYPE::IMG, uiImage + L"AirGliderExplan.png");
	resourcesMap_.emplace(SRC::AIR_GLIDER_CHECK_IMAGE, std::move(res));

	//エアグライダーの確認見出し
	res = std::make_unique<Resource>(Resource::TYPE::IMG, uiImage + L"AirGliderExplanText.png");
	resourcesMap_.emplace(SRC::AIR_GLIDER_CHECK_TITLE, std::move(res));

	//リザルトの1位画像
	res = std::make_unique<Resource>(Resource::TYPE::IMG, resultImage + L"1stImage.png");
	resourcesMap_.emplace(SRC::RANK_ONE, std::move(res));

	//リザルトの2位画像
	res = std::make_unique<Resource>(Resource::TYPE::IMG, resultImage + L"2ndImage.png");
	resourcesMap_.emplace(SRC::RANK_TWO, std::move(res));

	//リザルトの3位画像
	res = std::make_unique<Resource>(Resource::TYPE::IMG, resultImage + L"3rdImage.png");
	resourcesMap_.emplace(SRC::RANK_THREE, std::move(res));

	//リザルトの4位画像
	res = std::make_unique<Resource>(Resource::TYPE::IMG, resultImage + L"4thImage.png");
	resourcesMap_.emplace(SRC::RANK_FOUR, std::move(res));

	//リザルト背景画像
	res = std::make_unique<Resource>(Resource::TYPE::IMG, resultImage + L"ResultBack.png");
	resourcesMap_.emplace(SRC::RESULT_BACK, std::move(res));

	//カーテン画像
	res = std::make_unique<Resource>(Resource::TYPE::IMG, resultImage + L"ResultBackCurtain.png");
	resourcesMap_.emplace(SRC::CURTAIN, std::move(res));

#pragma endregion 


#pragma region マスク画像

	//UIのチャージゲージマスク画像
	res = std::make_unique<Resource>(Resource::TYPE::IMG, uiImage + L"ChargeGaugeMask.png");
	resourcesMap_.emplace(SRC::CHARGE_GAUGE_MASK, std::move(res));

#pragma endregion 


#pragma region モデル
	//キャラクターのパス
	std::wstring charaPath = PATH_MDL + L"Character/";

	//チキンのモデル
	res = std::make_unique<Resource>(Resource::TYPE::MODEL, charaPath + L"Chicken.mv1");
	resourcesMap_.emplace(SRC::CHICKEN, std::move(res));

	//マシンのモデルパス
	std::wstring machinePath = PATH_MDL + L"Machine/";

	//ワカバスター機体
	res = std::make_unique<Resource>(Resource::TYPE::MODEL, machinePath + L"Wakaba.mv1");
	resourcesMap_.emplace(SRC::WAKABA_STAR, std::move(res));

	//アーススター機体
	res = std::make_unique<Resource>(Resource::TYPE::MODEL, machinePath + L"Earth.mv1");
	resourcesMap_.emplace(SRC::EARTH_STAR, std::move(res));

	//バトルスター機体
	res = std::make_unique<Resource>(Resource::TYPE::MODEL, machinePath + L"Battle.mv1");
	resourcesMap_.emplace(SRC::BATTLE_STAR, std::move(res));

	//ボトルスター機体
	res = std::make_unique<Resource>(Resource::TYPE::MODEL, machinePath + L"Bottle.mv1");
	resourcesMap_.emplace(SRC::BOTTLE_STAR, std::move(res));

	//ステージオブジェクトのパス
	std::wstring stagePath = PATH_MDL + L"StageObject/";

	//ステージモデル
	res = std::make_unique<Resource>(Resource::TYPE::MODEL, stagePath + L"Stage.mv1");
	resourcesMap_.emplace(SRC::MAIN_STAGE, std::move(res));

	//エアグライダーステージモデル
	res = std::make_unique<Resource>(Resource::TYPE::MODEL, stagePath + L"GlideStage.mv1");
	resourcesMap_.emplace(SRC::GLIDE_STAGE, std::move(res));

	//エアグライダースタートステージモデル
	res = std::make_unique<Resource>(Resource::TYPE::MODEL, stagePath + L"GlideStartStage.mv1");
	resourcesMap_.emplace(SRC::GLIDE_START_STAGE, std::move(res));

	//草地モデル
	res = std::make_unique<Resource>(Resource::TYPE::MODEL, stagePath + L"Glass.mv1");
	resourcesMap_.emplace(SRC::GLASS, std::move(res));

	//ビルモデル
	res = std::make_unique<Resource>(Resource::TYPE::MODEL, stagePath + L"Building.mv1");
	resourcesMap_.emplace(SRC::BUILDING, std::move(res));

	//木モデル
	res = std::make_unique<Resource>(Resource::TYPE::MODEL, stagePath + L"Tree.mv1");
	resourcesMap_.emplace(SRC::TREE, std::move(res));

	//アイテムのパス
	std::wstring itemPath = PATH_MDL + L"Item/";

	//アイテムボックスモデル
	res = std::make_unique<Resource>(Resource::TYPE::MODEL, itemPath + L"ItemBox.mv1");
	resourcesMap_.emplace(SRC::ITEM_BOX, std::move(res));

	//大砲モデル
	res = std::make_unique<Resource>(Resource::TYPE::MODEL, itemPath + L"CannonBarrel.mv1");
	resourcesMap_.emplace(SRC::CANNON_MODEL, std::move(res));

	//大砲弾モデル
	res = std::make_unique<Resource>(Resource::TYPE::MODEL, itemPath + L"CannonShot.mv1");
	resourcesMap_.emplace(SRC::CANNON_SHOT_MODEL, std::move(res));

	//スカイドームのパス
	std::wstring skyDomePath = PATH_MDL + L"SkyDome/";

	//スカイドームモデル
	res = std::make_unique<Resource>(Resource::TYPE::MODEL, skyDomePath + L"SkyDome.mv1");
	resourcesMap_.emplace(SRC::SKY_DOME, std::move(res));

#pragma endregion 


#pragma region エフェクト

	//ブーストエフェクト
	res = std::make_unique<Resource>(Resource::TYPE::EFFEKSEER, PATH_EFF + L"Boost.efkproj");
	resourcesMap_.emplace(SRC::BOOST_EFFECT, std::move(res));

	//チャージエフェクト
	res = std::make_unique<Resource>(Resource::TYPE::EFFEKSEER, PATH_EFF + L"Charge.efkproj");
	resourcesMap_.emplace(SRC::CHARGE_EFFECT, std::move(res));

	//スピンエフェクト
	res = std::make_unique<Resource>(Resource::TYPE::EFFEKSEER, PATH_EFF + L"Spin.efkproj");
	resourcesMap_.emplace(SRC::SPIN_EFFECT, std::move(res));

	//爆発エフェクト
	res = std::make_unique<Resource>(Resource::TYPE::EFFEKSEER, PATH_EFF + L"Blast.efkefc");
	resourcesMap_.emplace(SRC::BLAST_EFFECT, std::move(res));

#pragma endregion

#pragma region サウンド

	//BGMのパス
	std::wstring bgmPath = PATH_SOUND + L"BGM/";

	//セレクトBGM
	res = std::make_unique<Resource>(Resource::TYPE::SOUND, bgmPath + L"SelectBGM.mp3");
	resourcesMap_.emplace(SRC::SELECT_BGM, std::move(res));

	//メインゲームBGM
	res = std::make_unique<Resource>(Resource::TYPE::SOUND, bgmPath + L"MainGameBGM.mp3");
	resourcesMap_.emplace(SRC::MAIN_GAME_BGM, std::move(res));

	//最終ミニゲームBGM
	res = std::make_unique<Resource>(Resource::TYPE::SOUND, bgmPath + L"LastGameBGM.mp3");
	resourcesMap_.emplace(SRC::LAST_GAME_BGM, std::move(res));
	
	//リザルトBGM
	res = std::make_unique<Resource>(Resource::TYPE::SOUND, bgmPath + L"ResultBGM.mp3");
	resourcesMap_.emplace(SRC::RESULT_BGM, std::move(res));

	//SEのパス
	std::wstring sePath = PATH_SOUND + L"SE/";

	//タイトルSE
	res = std::make_unique<Resource>(Resource::TYPE::SOUND, sePath + L"Title.mp3");
	resourcesMap_.emplace(SRC::TITLE_SE, std::move(res));

	//決定音
	res = std::make_unique<Resource>(Resource::TYPE::SOUND, sePath + L"Enter.mp3");
	resourcesMap_.emplace(SRC::ENTER_SE, std::move(res));

	//選択音
	res = std::make_unique<Resource>(Resource::TYPE::SOUND, sePath + L"SelectSe.mp3");
	resourcesMap_.emplace(SRC::SELECT_SE, std::move(res));

	//キャンセル音
	res = std::make_unique<Resource>(Resource::TYPE::SOUND, sePath + L"Cancel.mp3");
	resourcesMap_.emplace(SRC::CANCEL_SE, std::move(res));

	//エンジン音
	res = std::make_unique<Resource>(Resource::TYPE::SOUND, sePath + L"Engine.mp3");
	resourcesMap_.emplace(SRC::ENGINE_SE, std::move(res));

	//チャージ音
	res = std::make_unique<Resource>(Resource::TYPE::SOUND, sePath + L"Charge.mp3");
	resourcesMap_.emplace(SRC::CHARGE_SE, std::move(res));

	//チャージ完了音
	res = std::make_unique<Resource>(Resource::TYPE::SOUND, sePath + L"ChargeMax.mp3");
	resourcesMap_.emplace(SRC::CHARGE_MAX_SE, std::move(res));

	//ブースト音
	res = std::make_unique<Resource>(Resource::TYPE::SOUND, sePath + L"Boost.mp3");
	resourcesMap_.emplace(SRC::BOOST_SE, std::move(res));

	//ビーム音
	res = std::make_unique<Resource>(Resource::TYPE::SOUND, sePath + L"Beam.mp3");
	resourcesMap_.emplace(SRC::BEAM_SE, std::move(res));

	//大砲発射音
	res = std::make_unique<Resource>(Resource::TYPE::SOUND, sePath + L"Cannon.mp3");
	resourcesMap_.emplace(SRC::CANNON_SE, std::move(res));

	//ダメージ音
	res = std::make_unique<Resource>(Resource::TYPE::SOUND, sePath + L"Damage.mp3");
	resourcesMap_.emplace(SRC::DAMAGE_SE, std::move(res));

	//アイテム取得音
	res = std::make_unique<Resource>(Resource::TYPE::SOUND, sePath + L"GetItem.mp3");
	resourcesMap_.emplace(SRC::GET_ITEM_SE, std::move(res));
	
	//回復音
	res = std::make_unique<Resource>(Resource::TYPE::SOUND, sePath + L"Heal.mp3");
	resourcesMap_.emplace(SRC::HEAL_SE, std::move(res));

	//ジャンプ音
	res = std::make_unique<Resource>(Resource::TYPE::SOUND, sePath + L"Jump.mp3");
	resourcesMap_.emplace(SRC::JUMP_SE, std::move(res));

	//スピン音
	res = std::make_unique<Resource>(Resource::TYPE::SOUND, sePath + L"Spin.mp3");
	resourcesMap_.emplace(SRC::SPIN_SE, std::move(res));

	//カウントダウン音
	res = std::make_unique<Resource>(Resource::TYPE::SOUND, sePath + L"CountDown.mp3");
	resourcesMap_.emplace(SRC::COUNT_DOWN_SE, std::move(res));

	//タイムアップ音
	res = std::make_unique<Resource>(Resource::TYPE::SOUND, sePath + L"TimeUp.mp3");
	resourcesMap_.emplace(SRC::TIME_UP_SE, std::move(res));

#pragma endregion

}

void ResourceManager::Release(void)
{
	for (auto& p : loadedMap_)
	{
		p.second.Release();
	}

	loadedMap_.clear();
}

void ResourceManager::Destroy(void)
{
	Release();
	resourcesMap_.clear();
}

const Resource& ResourceManager::Load(SRC src)
{
	Resource& res = _Load(src);
	if (res.type_ == Resource::TYPE::NONE)
	{
		return dummy_;
	}
	return res;
}

int ResourceManager::LoadModelDuplicate(SRC src)
{
	Resource& res = _Load(src);
	if (res.type_ == Resource::TYPE::NONE)
	{
		return -1;
	}

	int duId = MV1DuplicateModel(res.handleId_);
	res.duplicateModelIds_.push_back(duId);

	return duId;
}

ResourceManager::ResourceManager(void)
{
}

ResourceManager::~ResourceManager(void)
{
}

Resource& ResourceManager::_Load(SRC src)
{

	// ロード済みチェック
	const auto& lPair = loadedMap_.find(src);
	if (lPair != loadedMap_.end())
	{
		return lPair->second;
	}

	// リソース登録チェック
	const auto& rPair = resourcesMap_.find(src);
	if (rPair == resourcesMap_.end())
	{
		// 登録されていない
		return dummy_;
	}

	// ロード処理
	rPair->second->Load();

	// 念のためコピーコンストラクタ
	loadedMap_.emplace(src, *rPair->second);

	return *rPair->second;

}
