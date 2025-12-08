#include "../pch.h"
#include "../Application.h"
#include "GameSetting.h"
#include "UIManager.h"

void UIManager::Init(void)
{
}

void UIManager::Draw(void)
{
	//空ならスキップ
	if (viewPort_.empty())return;

	for (int i = 0; i < GameSetting::GetInstance().GetUserNum(); i++)
	{
		//ビューポート
		const auto& vp = viewPort_[i];

		//画面領域を切り替え
		SetDrawArea(vp.x, vp.y, vp.x + vp.w, vp.y + vp.h);

		//各描画
		cameraDraw_[i]();
		uiDraw_[i]();
	}

	//元に戻す
	SetDrawArea(0, 0, Application::SCREEN_SIZE_X, Application::SCREEN_SIZE_Y);
}

UIManager::UIManager(void)
{
}

UIManager::~UIManager(void)
{
	viewPort_.clear();
}

void UIManager::CreateViewports(const int _playerCnt, const int _screenW, const int _screenH)
{
	//中身があるなら
	if (!viewPort_.empty())
	{
		//消して再生成
		viewPort_.clear();
	}

	if (_playerCnt == 1)
	{
		//全画面描画
		viewPort_.push_back({ 0,0,_screenW,_screenH });
	}
	else if (_playerCnt == 2)
	{
		//上下分割
		int h = _screenH / 2;
		viewPort_.push_back({ 0,0,_screenW,h});
		viewPort_.push_back({ 0,h,_screenW,h});
	}
	else if (_playerCnt >= 3)
	{
		//4分割
		int w = _screenW / 2;
		int h = _screenH / 2;
		viewPort_.push_back({ 0,0,w,h });
		viewPort_.push_back({ w,0,w,h });
		viewPort_.push_back({ 0,h,w,h });
		viewPort_.push_back({ w,h,w,h });
	}
}

const UIManager::Viewport& UIManager::GetViewPort(const int _index)const
{
	return viewPort_[_index];
}
