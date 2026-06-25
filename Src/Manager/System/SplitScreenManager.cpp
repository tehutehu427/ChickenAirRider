#include "../pch.h"
#include "../System/Camera.h"
#include "../System/SceneManager.h"
#include "../../Renderer/PixelMaterial.h"
#include "../../Renderer/PixelRenderer.h"
#include "SplitScreenManager.h"

void SplitScreenManager::LoadOutSide(void)
{
	//ピクセルシェーダーの作成
	//pixelMaterials_[static_cast<int>(SHADER_TYPE::DEFAULT)] = std::make_unique<PixelMaterial>(L"Default.cso", 1);
	//pixelMaterials_[static_cast<int>(SHADER_TYPE::MONO)] = std::make_unique<PixelMaterial>(L"Monotone.cso", 1);

	//関数設定
	setShader_[static_cast<int>(SHADER_TYPE::DEFAULT)] = &SplitScreenManager::SetDefault;
	setShader_[static_cast<int>(SHADER_TYPE::MONO)] = &SplitScreenManager::SetMono;
	setShader_[static_cast<int>(SHADER_TYPE::SEPIA)] = &SplitScreenManager::SetSepia;
	setShader_[static_cast<int>(SHADER_TYPE::SCAN_LINE)] = &SplitScreenManager::SetScanLine;
	setShader_[static_cast<int>(SHADER_TYPE::GOD_RAY)] = &SplitScreenManager::SetGodRay;

	updateShader_[static_cast<int>(SHADER_TYPE::DEFAULT)] = &SplitScreenManager::UpdateDefault;
	updateShader_[static_cast<int>(SHADER_TYPE::MONO)] = &SplitScreenManager::UpdateMono;
	updateShader_[static_cast<int>(SHADER_TYPE::SEPIA)] = &SplitScreenManager::UpdateSepia;
	updateShader_[static_cast<int>(SHADER_TYPE::SCAN_LINE)] = &SplitScreenManager::UpdateScanLine;
	updateShader_[static_cast<int>(SHADER_TYPE::GOD_RAY)] = &SplitScreenManager::UpdateGodRay;

	//レンダーの作成
	pixelRenderer_ = std::make_unique<PixelRenderer>();
}

void SplitScreenManager::Init(void)
{
	//描画情報の破棄
	Destroy();

	//初期化
	activeViewCount_ = 0;
}

void SplitScreenManager::UpdateShader(const int _index)
{
	(this->*updateShader_[static_cast<int>(splitViews_[_index].shader.type)])(_index);
}

void SplitScreenManager::Destroy(void)
{
	//分割スクリーンの破棄
	for (auto& view : splitViews_)
	{
		//スクリーンがあるなら
		if (view.renderScreen != -1)
		{
			//スクリーンの削除
			DeleteGraph(view.renderScreen);
			view.renderScreen = -1;
		}

		//その他要素の破棄
		view.camera.reset();
		view.viewport = {};
		view.material.reset();
		view.shader = {};
	}
}

void SplitScreenManager::BeginView(const int _index)
{
	//描画情報
	auto& view = splitViews_[_index];

	//スクリーンがないなら
	if (view.renderScreen == -1)return;

	//分割スクリーンの描画
	SetDrawScreen(view.renderScreen);

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

	//スクリーンのクリア
	ClearDrawScreen();

	//分割スクリーンの合成
	for (int i = 0; i < activeViewCount_; ++i)
	{
		//描画情報
		const auto& view = splitViews_[i];

		//スクリーンがないなら
		if (view.renderScreen == -1)continue;

		//レンダー用
		Vector2 renderPos = { view.viewport.x ,view.viewport.y};
		Vector2 renderSize = { view.viewport.width,view.viewport.height};

		//シェーダー描画
		pixelRenderer_->Draw(*view.material, renderPos, renderSize);

		//分割スクリーンの描画
		//DrawGraph(view.viewport.x, view.viewport.y, view.renderScreen, true);
	}
}

void SplitScreenManager::CreateSplitViews(const int _playerCnt, const int _screenW, const int _screenH)
{
	//元の数と同じなら何もしない
	if (activeViewCount_ == _playerCnt)return;

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

void SplitScreenManager::SetCamera(const int _index, const std::shared_ptr<Camera>& _camera)
{
	//カメラの設定
	splitViews_[_index].camera = _camera;
}

const SplitScreenManager::Viewport& SplitScreenManager::GetViewport(const int _index)const
{
	//ビューポートの取得
	return splitViews_[_index].viewport;
}

void SplitScreenManager::SetShader(const int _index, const SHADER_TYPE& _type)
{
	//描画情報
	auto& view = splitViews_[_index];

	//シェーダ生成
	view.shader.type = _type;
	(this->*setShader_[static_cast<int>(_type)])(_index);
	view.material->AddTextureBuf(view.renderScreen);
}

SplitScreenManager::SplitScreenManager(void)
	:activeViewCount_(0)
{
}

SplitScreenManager::~SplitScreenManager(void)
{
	//描画情報の破棄
	Destroy();
}

void SplitScreenManager::CreateView(const int _index, const int _x, const int _y, const int _width, const int _height)
{
	//描画情報の作成
	splitViews_[_index].viewport = { _x,_y,_width,_height };
	splitViews_[_index].renderScreen = MakeScreen(_width, _height, true);
	SetShader(_index, SHADER_TYPE::DEFAULT);
}

void SplitScreenManager::SetDefault(const int _index)
{
	splitViews_[_index].material = std::make_unique<PixelMaterial>(L"Default.cso", 1);
	splitViews_[_index].material->AddConstBuf({ 0.0f,0.0f,0.0f,0.0f });
}

void SplitScreenManager::SetMono(const int _index)
{
	splitViews_[_index].material = std::make_unique<PixelMaterial>(L"Monotone.cso", 1);
	splitViews_[_index].material->AddConstBuf({ 1.0f,1.0f,1.0f,1.0f });
}

void SplitScreenManager::SetSepia(const int _index)
{
	splitViews_[_index].material = std::make_unique<PixelMaterial>(L"Sepia.cso", 1);
	splitViews_[_index].material->AddConstBuf({ 1.0f,1.0f,1.0f,1.0f });
}

void SplitScreenManager::SetScanLine(const int _index)
{
	splitViews_[_index].material = std::make_unique<PixelMaterial>(L"ScanLine.cso", 1);
	splitViews_[_index].material->AddConstBuf({ 0.0f,0.0f,0.0f,0.0f });
}

void SplitScreenManager::SetGodRay(const int _index)
{
	splitViews_[_index].material = std::make_unique<PixelMaterial>(L"GodRay.cso", 1);
	splitViews_[_index].material->AddConstBuf({ 0.1f, 0.0f,0.95f, 0.7f });
}

void SplitScreenManager::UpdateDefault(const int _index)
{
}

void SplitScreenManager::UpdateMono(const int _index)
{
}

void SplitScreenManager::UpdateSepia(const int _index)
{
}

void SplitScreenManager::UpdateScanLine(const int _index)
{
	//デルタタイム
	float delta = SceneManager::GetInstance().GetDeltaTime();

	//シェーダー関係
	auto& shader = splitViews_[_index].shader;

	//カウンタ
	shader.cnt += delta;

	//定数バッファ用
	int constBufIndex = 0;
	FLOAT4 constBuf = { shader.cnt,0.0f,0.0f,0.0f };

	splitViews_[_index].material->SetConstBuf(constBufIndex, constBuf);
}

void SplitScreenManager::UpdateGodRay(const int _index)
{
}
