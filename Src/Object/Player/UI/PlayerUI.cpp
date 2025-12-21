#include "../pch.h"
#include "../Application.h"
#include "../../Manager/Game/GameSetting.h"
#include "PlayerUI.h"

void PlayerUI::Init(void)
{
	//初期化
	drawType_ = DRAW_TYPE::ACTION;
}

void PlayerUI::AddDraw(const DRAW_TYPE _type, std::function<void(void)> _draw)
{
	uiDraw_.emplace(_type, _draw);
}

void PlayerUI::SubDraw(const DRAW_TYPE _type)
{
	if (uiDraw_.find(_type) == uiDraw_.end())return;
	uiDraw_.erase(_type);
}

void PlayerUI::Draw(void)
{
	//描画処理がないならスキップ
	if (uiDraw_[drawType_] == nullptr)return;
		
	//ビューポート
	const auto& vp = viewPort_;

	//画面領域を切り替え
	SetDrawArea(vp.x, vp.y, vp.x + vp.w, vp.y + vp.h);

	//描画
	uiDraw_[drawType_]();

	//元に戻す
	SetDrawArea(0, 0, Application::SCREEN_SIZE_X, Application::SCREEN_SIZE_Y);
}

PlayerUI::PlayerUI(void)
{
	viewPort_ = {};
}

PlayerUI::~PlayerUI(void)
{
	uiDraw_.clear();
}

void PlayerUI::CreateViewports(const int _playerCnt, const int _screenW, const int _screenH)
{
	//中身があるなら
	//if (!viewPort_.empty())
	//{
	//	//消して再生成
	//	viewPort_.clear();
	//}

	if (_playerCnt == 1)
	{
		//全画面描画
		viewPort_ = { 0,0,_screenW,_screenH };
	}
	else if (_playerCnt == 2)
	{
		//上下分割
		int h = _screenH / 2;
		viewPort_ = { 0,0,_screenW,h};
	}
	else if (_playerCnt >= 3)
	{
		//4分割
		int w = _screenW / 2;
		int h = _screenH / 2;
		viewPort_ = { 0,0,w,h };
	}
}