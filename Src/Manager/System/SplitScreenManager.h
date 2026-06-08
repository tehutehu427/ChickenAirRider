#pragma once
#include<unordered_map>
#include<memory>
#include<array>
#include "../Common/Singleton.h"

class PixelMaterial;
class PixelRenderer;
class Camera;

class SplitScreenManager : public Singleton<SplitScreenManager>
{
	//継承元のコンストラクタ等にアクセスするため
	friend class Singleton<SplitScreenManager>;

public:

	//シェーダー種類
	enum class SHADER_TYPE
	{
		DEFAULT,	//通常
		MONO,		//分割スクリーン合成用
		MAX
	};

	//ビューポート
	struct Viewport
	{
		int x = 0, y = 0;				//ビューポート座標(左上)
		int width = 0, height = 0;		//ビューポートの大きさ
	};

	//分割描画情報
	struct SplitView
	{
		int renderScreen = -1;						//描画先のスクリーン
		std::weak_ptr<Camera> camera = {};			//カメラ
		Viewport viewport = {};						//ビューポート
		std::unique_ptr<PixelMaterial> materials_;	//ピクセルシェーダー
	};

	//画面分割の最大数
	static constexpr int MAX_SPLIT_NUM = 4;

	//初期化
	void LoadOutSide(void)override;

	//初期化
	void Init(void)override;

	//描画始め処理
	void BeginView(const int _playerIndex);

	//分割スクリーン合成
	void Composite(void);

	//ビューポートの生成
	void CreateSplitViews(const int _playerCnt, const int _screenW, const int _screenH);

	//分割スクリーン数の取得
	const int GetActiveViewCount(void)const { return activeViewCount_; }

	//カメラの設定
	void SetCamera(const int _playerIndex, const std::shared_ptr<Camera>& _camera);

	//ビューポートの取得
	const Viewport& GetViewport(const int _playerIndex)const;

private:

	//描画情報
	std::array<SplitView, MAX_SPLIT_NUM> splitViews_;

	//ピクセルシェーダー
	std::unique_ptr<PixelRenderer> pixelRenderer_;
	std::array<std::unique_ptr<PixelMaterial>, static_cast<int>(SHADER_TYPE::MAX)> pixelMaterials_;

	//有効な分割数
	int activeViewCount_;

	//コンストラクタ
	SplitScreenManager(void);

	//デストラクタ
	~SplitScreenManager(void)override;

	//削除
	void Destroy(void)override;

	//描画情報の作成
	void CreateView(const int _index, const int _x, const int _y, const int _width, const int _height);
};

