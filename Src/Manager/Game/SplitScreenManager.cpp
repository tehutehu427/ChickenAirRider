#include "../pch.h"
#include "../System/Camera.h"
#include "SplitScreenManager.h"

void SplitScreenManager::LoadOutSide(void)
{
}

void SplitScreenManager::Init(void)
{
	//初期化
	activeViewCount_ = 0;

	//描画スクリーンの削除
	for (auto& view : splitViews_)
	{
		//スクリーンがあるなら
		if (view.screen != -1)
		{
			//スクリーンの削除
			DeleteGraph(view.screen);
			view.screen = -1;
		}
	}

	//描画情報の初期化
	splitViews_.fill({});
}

void SplitScreenManager::BeginView(const int _playerIndex)
{
	//描画情報
	auto& view = splitViews_[_playerIndex];

	//スクリーンがないなら
	if (view.screen == -1)return;

	//分割スクリーンの描画
	SetDrawScreen(view.screen);

	//スクリーンのクリア
	ClearDrawScreen();

	//カメラの取得
	auto camera = view.camera.lock();

	//カメラがあるなら
	if (camera)
	{
		//カメラの設定
		camera->Apply();
	}
}

void SplitScreenManager::Composite(void)
{
	//通常スクリーン
	SetDrawScreen(DX_SCREEN_BACK);

	//分割スクリーンの合成
	for (int i = 0; i < activeViewCount_; ++i)
	{
		//描画情報
		const auto& view = splitViews_[i];

		//スクリーンがないなら
		if (view.screen == -1)continue;

		//分割スクリーンの描画
		DrawGraph(view.viewport.x, view.viewport.y, view.screen, true);
	}
}

void SplitScreenManager::CreateSplitViews(const int _playerCnt, const int _screenW, const int _screenH)
{
	//初期化
	Init();

	//有効な分割数の設定
	activeViewCount_ = std::min(_playerCnt, MAX_SPLIT_NUM);

	//プレイヤーの人数
	if (_playerCnt == 1)
	{
		//全画面描画
		CreateView(0, 0, 0, _screenW, _screenH);
	}
	else if (_playerCnt == 2)
	{
		//上下分割
		int h = _screenH / 2;

		//描画情報の作成
		CreateView(0, 0, 0, _screenW, h);
		CreateView(1, 0, h, _screenW, h);
	}
	else if (_playerCnt >= 3)
	{
		//4分割
		int w = _screenW / 2;
		int h = _screenH / 2;

		//描画情報の作成
		CreateView(0, 0, 0, w, h);
		CreateView(1, w, 0, w, h);
		CreateView(2, 0, h, w, h);
		CreateView(3, w, h, w, h);
	}
}

void SplitScreenManager::SetCamera(const int _playerIndex, const std::shared_ptr<Camera>& _camera)
{
	//カメラの設定
	splitViews_[_playerIndex].camera = _camera;
}

const SplitScreenManager::Viewport& SplitScreenManager::GetViewport(const int _playerIndex)
{
	//ビューポートの取得
	return splitViews_[_playerIndex].viewport;
}

SplitScreenManager::SplitScreenManager(void)
	:activeViewCount_(0)
{
}

SplitScreenManager::~SplitScreenManager(void)
{
}

void SplitScreenManager::CreateView(const int _index, const int _x, const int _y, const int _width, const int _height)
{
	//描画情報の作成
	splitViews_[_index].viewport = { _x,_y,_width,_height };
	splitViews_[_index].screen = MakeScreen(_width, _height);
}
