#include"../pch.h"
#include "Camera.h"
#include "ResourceManager.h"
#include "SoundManager.h"
#include "../Game/PlayerManager.h"
#include "../FpsControl/FpsControl.h"
#include "../Scene/SceneTitle.h"
#include "../Scene/SceneSelect.h"
#include "../Scene/Game/SceneGame.h"
#include "../Game/GameSetting.h"
#include "../Game/Timer.h"
#include"../Application.h"
#include"../Game/UIManager.h"
#include"SceneManager.h"

SceneManager::SceneManager(void)
{
	mainScreen_ = -1;	//メインスクリーンの初期化
	screenIndex_ = 0;	//分割スクリーンのインデックス初期化

	sceneId_ = SCENE_ID::NONE;
	waitSceneId_ = SCENE_ID::NONE;
	changeSceneState_ = CHANGE_SCENE_STATE::NONE;

	fader_ = nullptr;

	isSceneChanging_ = false;
	isSplitMode_ = false;

	cameras_.clear();
	splitScreens_.clear();

	// デルタタイム
	deltaTime_ = 1.0f / FPS;
	totalTime_ = -1.0f;

	//シーン生成用関数ポインタ
	createScene_[SCENE_ID::TITLE] = [this](void) {return CreateSceneTitle(); };
	createScene_[SCENE_ID::SELECT] = [this](void) {return CreateSceneSelect(); };
	createScene_[SCENE_ID::GAME] = [this](void) {return CreateSceneGame(); };

	//シーン変更
	changeScene_[CHANGE_SCENE_STATE::PUSH_BACK] = [this](void) {ChangeScenePushBack(); };
	changeScene_[CHANGE_SCENE_STATE::POP_BACK] = [this](void) {ChangeScenePopBack(); };
	changeScene_[CHANGE_SCENE_STATE::CHANGE_BACK] = [this](void) {ChangeSceneChangeBack(); };

	//フェード用関数ポインタ
	fadeState_[Fader::STATE::NONE] = [this](void) {};
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

	//初期シーン
	sceneId_ = SCENE_ID::TITLE;
	waitSceneId_ = SCENE_ID::NONE;

	//フェーダー
	fader_ = std::make_unique<Fader>();
	fader_->Init();
	
	//タイマー
	timer_ = std::make_unique<Timer>();
	timer_->Init(setting.GetTimeLimit());

	//カウント開始
	timer_->SetCountValid(false);
	timer_->SetCountView(false);

	// 初期のカメラは1つなので人数を初期化しておく
	setting.ResetPlayerNum();

	//シーンチェンジ中か
	isSceneChanging_ = true;

	// デルタタイム
	preTime_ = std::chrono::system_clock::now();

	// メインスクリーン
	mainScreen_ = MakeScreen(Application::SCREEN_SIZE_X, Application::SCREEN_SIZE_Y, true);
	
	//ウィンドウがアクティブ状態でなくとも処理を行う
	SetAlwaysRunFlag(true);

	// 3D用の設定
	Init3D();

	//初期シーンの挿入
	changeScene_[CHANGE_SCENE_STATE::PUSH_BACK]();

	screenIndex_ = 0;
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

	if (fader_->GetState() == Fader::STATE::NONE)
	{
		if(scene_.front() != nullptr)
		//先頭シーンの更新
		scene_.front()->Update();
	}

	//カメラの更新
	for (auto& c : cameras_)
	{
		c->Update();
	}

	//タイマーの更新
	timer_->Update();
}

void SceneManager::Draw(void)
{
	// 描画先グラフィック領域の指定
	// (３Ｄ描画で使用するカメラの設定などがリセットされる)
	SetDrawScreen(mainScreen_);

	// 画面を初期化
	ClearDrawScreen();

	if (isSplitMode_)
	{
		//複数画面の描画
		DrawMultiScreen();
	}
	else
	{
		// カメラ設定(単体カメラなので最初のカメラのみ)
		if (!cameras_.empty())
			cameras_.front()->SetBeforeDraw();

		// Effekseerにより再生中のエフェクトを更新する。
		UpdateEffekseer3D();

		// 描画
		for (auto& scene : scene_)
		{
			if (scene == nullptr)continue;
			scene->Draw();
		}

		//スカイドームの描画
		if (!cameras_.empty())
			cameras_.front()->DrawSkyDome();

		//UIは別々で描画
		if (sceneId_ == SCENE_ID::GAME)
			UIManager::GetInstance().Draw();

		// 主にポストエフェクト用
		if (!cameras_.empty())
			cameras_.front()->Draw();

		//タイマーの描画
		timer_->Draw();

		// Effekseerにより再生中のエフェクトを描画する。
		DrawEffekseer3D();

		// 暗転・明転
		fader_->Draw();
	}

	//背面スクリーンにメインスクリーンを描画
	SetDrawScreen(DX_SCREEN_BACK);

	// 画面を初期化
	ClearDrawScreen();

	// カメラ設定
	for (auto& camera : cameras_)
	{
		camera->CameraSetting();
	}

	//メインスクリーンを描画
	DrawGraph(0, 0, mainScreen_, false);
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
}

void SceneManager::PushScene(const SCENE_ID _sceneId, const bool _isFade)
{
	//リセット
	ResetChangeScene(_isFade);

	//待機シーンの変更
	waitSceneId_ = _sceneId;

	//シーン変更状態の変更
	changeSceneState_ = CHANGE_SCENE_STATE::PUSH_BACK;
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
	DeleteGraph(mainScreen_);
	for(auto & screen : splitScreens_){ DeleteGraph(screen); }
	
	//自身のインスタンス解放
	delete instance_;
	instance_ = nullptr;
}

void SceneManager::StartFadeIn(void)
{
	//フェードを明ける
	fader_->SetFade(Fader::STATE::FADE_IN);

	//シーンチェンジ
	isSceneChanging_ = false;
}

std::weak_ptr<Camera> SceneManager::GetCamera(const int _playerIndex) const
{
	return cameras_[_playerIndex];
}

void SceneManager::CreateCameras(const int _playerNum)
{
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

	//UIの分割
	UIManager::GetInstance().CreateViewports(_playerNum, Application::SCREEN_SIZE_X, Application::SCREEN_SIZE_Y);

	//カメラ生成
	for (int i = 0; i < _playerNum; i++)
	{
		std::shared_ptr<Camera> camera;
		camera = std::make_shared<Camera>(i);
		camera->Init();
		cameras_.push_back(camera);
	}

}

void SceneManager::CreateSplitScreen(const int _playerNum)
{
	//プレイヤー人数ごとのタイマー位置
	Vector2 timerPos[GameSetting::PLAYER_MAX_NUM] = {
		{ Application::SCREEN_HALF_X, 64 },									//1人
		{ 100, Application::SCREEN_HALF_Y },								//2人
		{ Application::SCREEN_HALF_X + Application::SCREEN_HALF_X / 2
		, Application::SCREEN_HALF_Y + Application::SCREEN_HALF_Y / 2},		//3人
		{ Application::SCREEN_HALF_X, Application::SCREEN_HALF_Y }			//4人
	};
	timer_->SetPos(timerPos[_playerNum - 1]);

	//引数が１以下
	// 最大人数を超える場合,
	// または引数と現在のスクリーン数が同じとき
	if (_playerNum <= 1 || 
		_playerNum > 4/* ||
		splitScreens_.size() == _playerNum*/)
	{
		isSplitMode_ = false;	//分割しない
		return;					//生成しない
	}
	//空じゃない場合
	else if (!splitScreens_.empty())
	{
		//中身削除
		splitScreens_.clear();
	}

	//分割を行う
	isSplitMode_ = true;

	int createNum = _playerNum;	//生成数
	int divX = 1;				//Xの分割数
	int divY = 1;				//Yの分割数
	
	//人数が条件以上の場合
	if (_playerNum >= CASE_VALUE)
	{
		createNum = 4;	//最大人数分生成
		divX++;			//画面分割数増加
		divY++;			
	}
	else if (_playerNum == 2)
	{
		divY++;
	}

	//スクリーン生成
	for (int i = 0; i < createNum; i++)
	{
		int screen = MakeScreen(
			Application::SCREEN_SIZE_X / divX,
			Application::SCREEN_SIZE_Y / divY,
			true);
		splitScreens_.push_back(screen);
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

void SceneManager::DrawMultiScreen()
{
	const int userNum = GameSetting::GetInstance().GetUserNum();

	//描画位置（分割スクリーンの左上位置）
	Vector2 screenPos[GameSetting::PLAYER_MAX_NUM] =
	{
		{ 0, 0 },													// 左上
		{ 0, Application::SCREEN_HALF_Y},							// 左下
		{ Application::SCREEN_HALF_X, 0 },							// 右上
		{ Application::SCREEN_HALF_X, Application::SCREEN_HALF_Y }	// 右下
	};

	//3人以上なら
	if (userNum >= CASE_VALUE)
	{
		//2Pを右上に、3Pを左下に入れ替える
		Vector2 savePos = screenPos[1];
		screenPos[1] = screenPos[2];
		screenPos[2] = savePos;
	}

	//スクリーン数分描画
	for (int i = 0; i < splitScreens_.size(); i++)
	{
		//プレイ人数が3人の時の4つ目の画面を1Pの画面を表示する
		screenIndex_ = i;	//分割スクリーンのインデックス
		int index = i;
		if (CASE_VALUE == userNum &&
			index == CASE_VALUE)
		{
			index = 1;
			screenIndex_ = 1;
		}

		//分割スクリーンの設定
		SetDrawScreen(splitScreens_[index]);

		//画面クリア
		ClearDrawScreen();

		// カメラ設定
		cameras_[index]->SetBeforeDraw();

		// Effekseerにより再生中のエフェクトを更新する。
		UpdateEffekseer3D();

		// 描画
		for (const auto& scene : scene_)
		{
			scene->Draw();
		}

		//スカイドームの描画
		if (!cameras_.empty())
			cameras_[index]->DrawSkyDome();

		//UIは別々で描画
		UIManager::GetInstance().Draw(index);

		// 主にポストエフェクト用
		cameras_[index]->Draw(index);

		// Effekseerにより再生中のエフェクトを描画する。
		DrawEffekseer3D();

		// 暗転・明転
		fader_->Draw();
	}

	//メインスクリーンへ描画
	SetDrawScreen(mainScreen_);


	for (int i = 0; i < splitScreens_.size(); i++)
	{
		//分割したスクリーンを描画
		DrawGraph(screenPos[i].x, screenPos[i].y, splitScreens_[i], true);
	}

	//タイマーの描画
	timer_->Draw();

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

std::unique_ptr<SceneBase> SceneManager::CreateSceneGame(void)
{
	//ゲームシーン生成
	return std::move(std::make_unique<SceneGame>());
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
	scene_.back() = std::move(createScene_[sceneId_]());

	//初期化
	scene_.back()->Load();
	scene_.back()->Init();
}

void SceneManager::FadeOut(void)
{
	// 暗転中
	if (fader_->IsEnd())
	{
		//シーンIDの変更
		sceneId_ = waitSceneId_;

		//シーンに合わせて生成数を設定
		const int createNum = GameSetting::GetInstance().GetUserNum();

		//ゲームの時のみ分割
		if (sceneId_ == SCENE_ID::GAME)
		{
			//カメラ生成
			CreateCameras(createNum);

		}

		//分割スクリーン生成
		CreateSplitScreen(createNum);

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
	}
}
