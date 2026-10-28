#include "../../pch.h"
#include "../../Application.h"
#include "FontManager.h"

const std::wstring FontManager::GetFontName(const FONT_TYPE& _type)const
{
	return fontName_[static_cast<int>(_type)];
}

const int FontManager::GetFontHandle(const FONT_TYPE& _type, const int _size, const int _thick)
{
	//フォント
	auto& font = fontHandle_[static_cast<int>(_type)];

	//サイズと厚さを元にキーにする
	auto pairKey = std::make_pair(_size, _thick);

	//同じサイズと厚さであるか
	auto itr = font.find(pairKey);

	//既に存在するなら
	if (itr != font.end())
	{
		//既にあるものを返す
		return itr->second;
	}

	//ないなら新たに作る
	const auto& name = fontName_[static_cast<int>(_type)].c_str();
	int handle = CreateFontToHandle(name, _size, _thick, 0);

	//格納
	font.emplace(pairKey, handle);

	//返す
	return handle;
}

FontManager::FontManager(void)
{
}

FontManager::~FontManager(void)
{
}

void FontManager::LoadOutSide(void)
{
	//フォント名(プロパティ詳細のタイトル名を参照)
	fontName_[static_cast<int>(FONT_TYPE::DOT)] = L"ベストテンDOT";
	fontName_[static_cast<int>(FONT_TYPE::BOKUTATI)] = L"ぼくたちのゴシック";
	fontName_[static_cast<int>(FONT_TYPE::HANAZOME)] = L"はなぞめフォント";
	fontName_[static_cast<int>(FONT_TYPE::PENGS)] = L"PENGS";
	fontName_[static_cast<int>(FONT_TYPE::LOGO)] = L"コーポレート・ロゴ ver3 Bold";
	fontName_[static_cast<int>(FONT_TYPE::BANANA)] = L"YDW バナナスリップplus plus";

	// フォントパス初期化
	fontPath_[static_cast<int>(FONT_TYPE::DOT)] = L"FontDot.otf";
	fontPath_[static_cast<int>(FONT_TYPE::BOKUTATI)] = L"bokutachi.otf";
	fontPath_[static_cast<int>(FONT_TYPE::HANAZOME)] = L"はなぞめフォント.ttf";
	fontPath_[static_cast<int>(FONT_TYPE::PENGS)] = L"PENGS.ttf";
	fontPath_[static_cast<int>(FONT_TYPE::LOGO)] = L"Corporate-Logo-Bold-ver3.otf";
	fontPath_[static_cast<int>(FONT_TYPE::BANANA)] = L"YDWbananaslipplus.otf";

	//フォントをシステムに登録
	const int FONT_MAX = static_cast<int>(FONT_TYPE::MAX);
	for (int i = 0; i < FONT_MAX; i++)
	{
		//登録
		AddFontResourceEx((Application::PATH_FONT + fontPath_[i]).c_str(), FR_PRIVATE, NULL);
	}
}

void FontManager::Destroy()
{
	// フォント登録解除
	const int FONT_MAX = static_cast<int>(FONT_TYPE::MAX);
	for (int i = 0; i < FONT_MAX; i++)
	{
		//解除
		RemoveFontResourceEx((Application::PATH_FONT + fontPath_[i]).c_str(), FR_PRIVATE, NULL);
	}
}