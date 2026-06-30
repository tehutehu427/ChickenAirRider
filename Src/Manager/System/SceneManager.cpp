#include "../pch.h"
#include "../Application.h"
#include "../FpsControl/FpsControl.h"
#include "../Scene/SceneTitle.h"
#include "../Scene/Select/SceneSelect.h"
#include "../Scene/Select/SceneOption.h"
#include "../Scene/Game/SceneGame.h"
#include "../Scene/SceneResult.h"
#include "../Game/HUDManager.h"
#include "../Game/GameSetting.h"
#include "../Game/GlobalUIManager.h"
#include "../Game/Timer.h"
#include "SplitScreenManager.h"
#include "Camera.h"
#include "SceneManager.h"

SceneManager::SceneManager(void)
{
	sceneId_ = SCENE_ID::NONE;
	waitSceneId_ = SCENE_ID::NONE;
	changeSceneState_ = CHANGE_SCENE_STATE::NONE;

	fader_ = nullptr;

	isSceneChanging_ = false;

	cameras_.clear();

	// デルタタイム
	deltaTime_ = 1.0f / FPS;
	totalTime_ = -1.0f;

	//シーン生成用関数ポインタ
	createScene_[SCENE_ID::TITLE] = [this](void) {return CreateSceneTitle(); };
	createScene_[SCENE_ID::SELECT] = [this](void) {return CreateSceneSelect(); };
	createScene_[SCENE_ID::OPTION] = [this](void) {return CreateSceneOption(); };
	createScene_[SCENE_ID::GAME] = [this](void) {return CreateSceneGame(); };
	createScene_[SCENE_ID::RESULT] = [this](void) {return CreateSceneResult(); };

	//シーン変更
	changeScene_[CHANGE_SCENE_STATE::PUSH_BACK] = [this](void) {ChangeScenePushBack(); };
	changeScene_[CHANGE_SCENE_STATE::POP_BACK] = [this](void) {ChangeScenePopBack(); };
	changeScene_[CHANGE_SCENE_STATE::CHANGE_BACK] = [this](void) {ChangeSceneChangeBack(); };

	//フェード用関数ポインタ
	fadeState_[Fader::STATE::NONE] = [this](void) {FadeNone(); };
	fadeState_[Fader::STATE::FADE_IN] = [this](void) {FadeIn(); };
	fadeState_[Fader::STATE::FADE_OUT] = [this](void) {FadeOut(); };
}

SceneManager::~SceneManager(void)
{
}

void SceneManager::Init(void)
{
	//ゲーム設定
	auto& setting = GameSetting::GetInstance();

	//分割スクリーン
	SplitScreenManager::CreateInstance(SingletonRegistry::DESTROY_TIMING::ALL_END);

	//画面UIの生成
	HUDManager::CreateInstance(SingletonRegistry::DESTROY_TIMING::ALL_END);

	//初期シーン
	sceneId_ = SCENE_ID::TITLE;
	waitSceneId_ = SCENE_ID::TITLE;
	changeSceneState_ = CHANGE_SCENE_STATE::PUSH_BACK;

	//フェーダー
	fader_ = std::make_unique<Fader>();
	fader_->Init();
	
	// 初期のカメラは1つなので人数を初期化しておく
	setting.ResetPlayerNum();

	//シーンチェンジ中か
	isSceneChanging_ = false;

	// デルタタイム
	preTime_ = std::chrono::system_clock::now();
	
	//ウィンドウがアクティブ状態でなくとも処理を行う
	SetAlwaysRunFlag(true);

	// 3D用の設定
	Init3D();

	//初期シーンの挿入
	changeScene_[CHANGE_SCENE_STATE::PUSH_BACK]();

	//初期カメラ
	CreateCameraAndSplitScreens(1);
}

void SceneManager::Init3D(void)
{
	// 背景色設定
	SetBackgroundColor(0, 0, 0);

	// Zバッファを有効にする
	SetUseZBuffer3D(true);

	// Zバッファへの書き込みを有効にする
	SetWriteZBuffer3D(true);

	// バックカリングを有効にする
	SetUseBackCulling(true);

	// ライトの設定
	SetUseLighting(true);
	
	// ライトの設定
	ChangeLightTypeDir({ 0.3f, -0.7f, 0.8f });

	// フォグ設定
	SetFogEnable(false);
	SetFogColor(5, 5, 5);
	SetFogStartEnd(10000.0f, 20000.0f);

}

void SceneManager::Update(void)
{
	//シーンが空なら処理スキップ
	if (scene_.empty())  return; 

	// デルタタイム
	auto nowTime = std::chrono::system_clock::now();
	//deltaTime_ = static_cast<float>(
	//	std::chrono::duration_cast<std::chrono::nanoseconds>(nowTime - preTime_).count() / 1000000000.0);
	deltaTime_ = 1.0f / FPS;
	preTime_ = nowTime;
	totalTime_ += deltaTime_;

	//シーンチェンジ中なら
	if (isSceneChanging_)
	{
		//フェード
		Fade();
	}

	//フェーダー更新
	fader_->Update();

	//フェード中ではない
	if (fader_->GetState() == Fader::STATE::NONE)
	{
		//全体UIの更新
		GlobalUIManager::GetInstance().Update();

		if(scene_.back() != nullptr)
		//先頭シーンの更新
		scene_.back()->Update();
	}

	//カメラの更新
	for (auto& c : cameras_)
	{
		c->Update();
	}
}

void SceneManager::Draw(void)
{
	// Effekseerにより再生中のエフェクトを更新する。
	UpdateEffekseer3D();

	//分割スクリーン
	auto& split = SplitScreenManager::GetInstance();

	for (int i = 0; i < split.GetActiveViewCount(); i++)
	{
		//シェーダー関係の更新
		split.UpdateShader(i);

		//分割スクリーン描画開始
		split.BeginView(i);

		//シーンの描画
		for (auto& s : scene_) 
		{
			//シーンが空じゃないなら描画
			if(s != nullptr) s->Draw(*cameras_[i]);
		}

		//Effekseerにより再生中のエフェクトを描画する。
		DrawEffekseer3D();
	}

	//分割スクリーン合成
	split.Composite();

	//HUDの描画
	HUDManager::GetInstance().Draw();

	//全体UIの描画
	GlobalUIManager::GetInstance().Draw();

	//フェードの描画
	fader_->Draw();
}

void SceneManager::ChangeScene(const SCENE_ID _sceneId, const bool _isReset, const bool _isFade)
{
	//リセットの有無
	if (_isReset)
	{
		//リセット
		ResetChangeScene(_isFade);
	}

	//待機シーンの変更
	waitSceneId_ = _sceneId;

	//シーンが空なら
	if (scene_.empty())
	{
		//新しく作る
		changeSceneState_ = CHANGE_SCENE_STATE::PUSH_BACK;
	}
	//シーンが存在するなら
	else
	{
		//最前列を変更
		changeSceneState_ = CHANGE_SCENE_STATE::CHANGE_BACK;
	}
}

void SceneManager::ChangeAllScene(const SCENE_ID _sceneId)
{
	//フェード開始
	StartFadeIn();

	//シーンの全消去
	scene_.clear();

	//待機シーンの変更
	waitSceneId_ = _sceneId;

	//シーン変更状態の変更
	changeSceneState_ = CHANGE_SCENE_STATE::PUSH_BACK;

	//シーン遷移中
	isSceneChanging_ = true;
}

void SceneManager::PushScene(const SCENE_ID _sceneId, const bool _isFade)
{
	//リセット
	ResetChangeScene(_isFade);

	//待機シーンの変更
	waitSceneId_ = _sceneId;

	//シーン変更状態の変更
	changeSceneState_ = CHANGE_SCENE_STATE::PUSH_BACK;

	//シーン遷移中
	isSceneChanging_ = true;
}

void SceneManager::PopScene(void)
{
	//リセット
	ResetChangeScene(false);

	//シーンが空にならないなら
	if (scene_.size() > 1)
	{
		//取り出す
		changeSceneState_ = CHANGE_SCENE_STATE::POP_BACK;
	}

	//シーン遷移中
	isSceneChanging_ = true;
}

void SceneManager::ResetScene(void)
{
	// 現在のシーンを解放
	if (!scene_.empty())
	{
		scene_.back().reset();
	}
}

void SceneManager::Destroy(void)
{
	//スクリーンの解放
	//DeleteGraph(mainScreen_);
	//for(auto & screen : splitScreens_){ DeleteGraph(screen); }
}

void SceneManager::StartFadeIn(void)
{
	//フェードを明ける
	fader_->SetFade(Fader::STATE::FADE_IN);

	//シーンチェンジ
	isSceneChanging_ = true;
}

std::weak_ptr<Camera> SceneManager::GetCamera(const int _playerIndex) const
{
	return cameras_[_playerIndex];
}

void SceneManager::CreateCameraAndSplitScreens(const int _playerNum)
{
	//分割スクリーン
	auto& split = SplitScreenManager::GetInstance();

	//UIの分割
	split.CreateSplitViews(_playerNum, Application::SCREEN_SIZE_X, Application::SCREEN_SIZE_Y);

	//現在のカメラの数が引数と同じ場合
	if (cameras_.size() == _playerNum)
	{
		//処理は実行しない
		return;
	}
	//カメラが空じゃない場合
	else if (!cameras_.empty())
	{
		//カメラの中身を削除
		cameras_.clear();
	}

	//カメラ生成
	for (int i = 0; i < _playerNum; i++)
	{
		//カメラ生成
		std::shared_ptr<Camera> camera;
		camera = std::make_shared<Camera>(i);
		camera->Init();
		cameras_.push_back(camera);

		//UIの分割にカメラを登録
		split.SetCamera(i, camera);
	}

}

void SceneManager::ResetDeltaTime(void)
{
	deltaTime_ = 1.0f / 60.0f;
	preTime_ = std::chrono::system_clock::now();
}

void SceneManager::DoChangeScene(SCENE_ID sceneId)
{
	//// リソースの解放
	//ResourceManager::GetInstance().Release();	
	//SoundManager::GetInstance().Release();	

	//// シーンを変更する
 //	sceneId_ = sceneId;
	//
	////初期化
	//screenIndex_ = 0;		

	//// 現在のシーンを解放
	//if (scene_ != nullptr)
	//{
	//	scene_.reset();
	//}

	////シーンに合わせて生成数を設定
	////const int createNum = (sceneId == SCENE_ID::MULTI) ? DateBank::GetInstance().GetPlayerNum() : 1;

	////カメラ生成
	////CreateCameras(createNum);

	////分割スクリーン生成
	////CreateSplitScreen(createNum);

	////シーンを生成
	//switch (sceneId_)
	//{
	//case SCENE_ID::TITLE:
	//	scene_ = std::make_unique<SceneTitle>();
	//	break;

	//case SCENE_ID::SELECT:
	//	scene_ = std::make_unique<SceneTitle>();
	//	break;

	//case SCENE_ID::GAME:
	//	scene_ = std::make_unique<SceneTitle>();
	//	break;
	//}

	////読み込み
	//scene_->Load();

	////デルタタイムリセット
	//ResetDeltaTime();

	////シーンID初期化
	//waitSceneId_ = SCENE_ID::NONE;
}

void SceneManager::ResetChangeScene(const bool _isFade)
{
	// リソースの解放
	//ResourceManager::GetInstance().Release();	
	//SoundManager::GetInstance().Release();	

	//デルタタイムリセット
	ResetDeltaTime();

	//フェードの有無
	if (_isFade)
	{
		// フェードアウト(暗転)を開始する
		fader_->SetFade(Fader::STATE::FADE_OUT);
		isSceneChanging_ = true;
	}
}

void SceneManager::Fade(void)
{
	//フェードの有無
	Fader::STATE fState = fader_->GetState();

	//フェード処理
	fadeState_[fState]();
}

std::unique_ptr<SceneBase> SceneManager::CreateSceneTitle(void)
{
	//タイトルシーン生成   
	return std::move(std::make_unique<SceneTitle>());
}

std::unique_ptr<SceneBase> SceneManager::CreateSceneSelect(void)
{
	//セレクトシーン生成
	return std::move(std::make_unique<SceneSelect>());
}

std::unique_ptr<SceneBase> SceneManager::CreateSceneOption(void)
{
	//オプションシーン生成
	return std::move(std::make_unique<SceneOption>());
}

std::unique_ptr<SceneBase> SceneManager::CreateSceneGame(void)
{
	//ゲームシーン生成
	return std::move(std::make_unique<SceneGame>());
}

std::unique_ptr<SceneBase> SceneManager::CreateSceneResult(void)
{
	return std::move(std::make_unique<SceneResult>());
}

void SceneManager::ChangeScenePushBack(void)
{
	//シーンの末尾追加
	scene_.push_back(std::move(createScene_[sceneId_]()));

	//初期化
	scene_.back()->Load();
	scene_.back()->Init();
}

void SceneManager::ChangeScenePopBack(void)
{
	//シーンの先頭削除
	scene_.pop_back();
}

void SceneManager::ChangeSceneChangeBack(void)
{
	//シーンの先頭変更
	scene_.pop_back();
	scene_.push_back(std::move(createScene_[sceneId_]()));

	//初期化
	scene_.back()->Load();
	scene_.back()->Init();
}

void SceneManager::FadeNone(void)
{
	//シーンIDの変更
	sceneId_ = waitSceneId_;

	//シーン遷移
	changeScene_[changeSceneState_]();

	//待機シーンIDの初期化
	waitSceneId_ = SCENE_ID::NONE;

	//シーン遷移終了
	isSceneChanging_ = false;
}

void SceneManager::FadeOut(void)
{
	// 暗転中
	if (fader_->IsEnd())
	{
		//シーンIDの変更
		sceneId_ = waitSceneId_;

		//シーンに合わせて生成数を設定
		int createNum = 1; 

		//ゲームの時のみ分割
		if (sceneId_ == SCENE_ID::GAME)
		{
			//プレイヤー人数
			createNum = GameSetting::GetInstance().GetUserNum();

			//プレイヤー人数ごとのタイマー位置
			Vector2 timerPos[GameSetting::PLAYER_MAX_NUM] = {
				{ Application::SCREEN_HALF_X, 64 },									//1人
				{ 100, Application::SCREEN_HALF_Y },								//2人
				{ Application::SCREEN_HALF_X + Application::SCREEN_HALF_X / 2
				, Application::SCREEN_HALF_Y + Application::SCREEN_HALF_Y / 2},		//3人
				{ Application::SCREEN_HALF_X, Application::SCREEN_HALF_Y }			//4人
			};

			//タイマーの位置
			GlobalUIManager::GetInstance().GetTimer().SetPos(timerPos[createNum - 1]);
		}

		//カメラ生成と分割スクリーンの設定
		CreateCameraAndSplitScreens(createNum);

		//シーンの遷移
		changeScene_[changeSceneState_]();

		//待機シーンIDの初期化
		waitSceneId_ = SCENE_ID::NONE;

		// 暗転から明転へ
		fader_->SetFade(Fader::STATE::FADE_IN);
	}
}

void SceneManager::FadeIn(void)
{
	// 明転中
	if (fader_->IsEnd())
	{
		// 明転が終了したら、フェード処理終了
		fader_->SetFade(Fader::STATE::NONE);

		//シーン遷移終了
		isSceneChanging_ = false;
	}
}
