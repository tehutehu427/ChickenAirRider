#pragma once
#include "../Common/Singleton.h"
#include "../Common/Vector2.h"

class PlayerUI
{
public:

	enum class DRAW_TYPE
	{
		ACTION,
	};

	//ビューポート
	struct Viewport
	{
		int x = 0, y = 0,		//左上座標
			w = 0, h = 0;		//サイズ
	};

	//正規化UI座標
	struct NormalizedPos
	{
		float x = 0.0f, y = 0.0f;		//UI座標(0.0～1.0)
	};

	//コンストラクタ
	PlayerUI(void);

	//デストラクタ
	~PlayerUI(void);

	//初期化
	void Init(void);

	//描画追加
	void AddDraw(const DRAW_TYPE _type, std::function<void(void)> _draw);
	void SubDraw(const DRAW_TYPE _type);

	/// @brief 描画
	/// @param _index プレイヤー番号
	void Draw(void);

	//ビューポートの生成
	void CreateViewports(const int _playerCnt, const int _screenW, const int _screenH);

	//ビューポートごとのサイズ
	const Viewport& GetViewPort(void)const { return viewPort_; }

private:

	//ビューポート
	Viewport viewPort_;

	//描画用関数ポインタ
	std::unordered_map<DRAW_TYPE, std::function<void(void)>> uiDraw_;
};

