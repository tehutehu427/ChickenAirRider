#include "../../pch.h"
#include "ResourceManager.h"
#include "FontManager.h"
#include "TextManager.h"

void TextManager::Init(void)
{
}

void TextManager::DrawTextToImage(const std::wstring& _str, const int _x, const int _y, const int _size, const int _thick, const FONT_TYPE _font)
{
	//フォントを取得
	auto& fontMng = FontManager::GetInstance();
	const int font = fontMng.GetFontHandle(_font, _size, _thick);

	//まだ描画していない文章
	std::wstring remaining = _str;

	//位置
	int drawX = _x;

	//フォントサイズ
	int fontSize = _size;

	//描画し終わっていないなら
	while (!remaining.empty())
	{
		//文章から置き換えれるものを探す
		size_t foundPos = std::wstring::npos;
		std::wstring foundKey;
		int foundImage = -1;

		//登録されている文字列を調べる
		for (const auto& textImage : textImages_)
		{
			//文字列キー
			const std::wstring& key = textImage.text;

			size_t pos = remaining.find(key);

			//見つかった
			if (pos != std::wstring::npos)
			{
				//まだ見つけていない　または　より前にあるなら
				if (foundPos == std::wstring::npos || pos < foundPos)
				{
					//保存
					foundPos = pos;
					foundKey = key;
					foundImage = textImage.image;
				}
			}
		}

		//置き換えれる文字がなかった
		if (foundPos == std::wstring::npos)
		{
			DrawStringToHandle()
		}
	}
}

TextManager::TextManager(void)
{
}

TextManager::~TextManager(void)
{
}

void TextManager::LoadOutSide(void)
{
	//リソース
	auto& res = ResourceManager::GetInstance();

	//ここで画像変換したいテキストとその画像を保存しておく
	textImages_.push_back({ L"Aボタン",res.Load(ResourceManager::SRC::A_BUTTON).handleId_ });
	textImages_.push_back({ L"Bボタン",res.Load(ResourceManager::SRC::B_BUTTON).handleId_ });
	textImages_.push_back({ L"Xボタン",res.Load(ResourceManager::SRC::X_BUTTON).handleId_ });
	textImages_.push_back({ L"Yボタン",res.Load(ResourceManager::SRC::Y_BUTTON).handleId_ });
	textImages_.push_back({ L"左クリック",res.Load(ResourceManager::SRC::MOUSE_LEFT).handleId_ });
	textImages_.push_back({ L"右クリック",res.Load(ResourceManager::SRC::MOUSE_RIGHT).handleId_ });
	textImages_.push_back({ L"マウスを動かす",res.Load(ResourceManager::SRC::MOUSE_MOVE).handleId_ });
}