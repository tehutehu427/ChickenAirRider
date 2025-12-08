#pragma once
#include "../Common/Singleton.h"
#include "../Common/Vector2.h"

class UIManager : public Singleton<UIManager>
{
	//継承元のコンストラクタ等にアクセスするため
	friend class Singleton<UIManager>;

public:

	//ビューポート
	struct Viewport
	{
		int x, y,		//左上座標
			w, h;		//サイズ
	};

	//正規化UI座標
	struct NormalizedPos
	{
		float x, y;		//UI座標(0.0～1.0)
	};

	//初期化
	void Init(void)override;

	//マネージャーに追加する描画処理
	void AddCameraDraw(std::function<void(void)> _draw) { cameraDraw_.push_back(_draw); }
	void AddUIDraw(std::function<void(void)> _draw) { uiDraw_.push_back(_draw); }

	/// @brief 描画
	void Draw(void);

	//ビューポートの生成
	void CreateViewports(const int _playerCnt, const int _screenW, const int _screenH);

	//ビューポートごとのサイズ
	const Viewport& GetViewPort(const int _index)const;

private:

	//コンストラクタ
	UIManager(void);

	//デストラクタ
	~UIManager(void)override;

	//ビューポート
	std::vector<Viewport>viewPort_;

	//描画用関数ポインタ
	std::vector<std::function<void(void)>> cameraDraw_;
	std::vector<std::function<void(void)>> uiDraw_;
};

