#include "../../pch.h"
#include "ResourceManager.h"
#include "FontManager.h"
#include "TextManager.h"

void TextManager::Init(void)
{
}

void TextManager::DrawTextToImage(const std::wstring& _str, const int _x, const int _y, const int _size, const int _thick, const int _color, const FONT_TYPE _font)const
{
	//フォントを取得
	auto& fontMng = FontManager::GetInstance();
	const int font = fontMng.GetFontHandle(_font, _size, _thick);

	//まだ描画していない文章
	std::wstring remaining = _str;

	//位置
	int drawX = _x;
	int drawY = _y;

	//フォントサイズ
	int fontSize = _size;

	//行間
	const int BETWEEN = 5;
	int lineHeight = fontSize + BETWEEN;

	//描画し終わっていないなら
	while (!remaining.empty())
	{
		//改行を探す
		size_t newLinePos = remaining.find('\n');

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

		//改行がテキスト画像より先にあるなら
		if (newLinePos != std::wstring::npos && (foundPos == std::wstring::npos || newLinePos < foundPos))
		{
			//改行より前の文字
			std::wstring before = remaining.substr(0, newLinePos);

			//改行までに文字があるなら描画
			if (!before.empty())
			{
				//描画
				DrawStringToHandle(drawX, drawY, before.c_str(), _color, font);

				//描画した文字幅だけ右にずらす
				drawX += GetDrawStringWidthToHandle(before.c_str(), -1, font);
			}

			//改行
			drawX = _x;
			drawY += lineHeight;

			//改行文字を削除
			remaining = remaining.substr(newLinePos + 1);

			//スキップ
			continue;
		}

		//置き換えれる文字がなかった
		if (foundPos == std::wstring::npos)
		{
			//文章描画
			DrawStringToHandle(drawX, drawY, remaining.c_str(), _color, font);
			
			//終了
			break;
		}

		//置き換える文字より前の文章
		std::wstring before = remaining.substr(0, foundPos);

		//文章があるなら
		if (!before.empty())
		{
			//文章描画
			DrawStringToHandle(drawX, drawY, before.c_str(), _color, font);

			//描画位置をずらす
			drawX += GetDrawStringWidthToHandle(before.c_str(), -1, font);
		}

		//画像の描画
		int originalWidth;
		int originalHeight;

		//画像サイズ
		GetGraphSize(foundImage, &originalWidth, &originalHeight);

		//フォントと比べた画像側の比率
		const float IMAGE_SCALE = 1.2f;

		//フォントサイズに画像の高さを合わせる
		int imageHeight = static_cast<int>(fontSize * IMAGE_SCALE);

		//幅を合わせる
		int imageWidth = static_cast<int>(static_cast<float>(originalWidth) * imageHeight / originalHeight);

		//文字の中央に画像を配置
		int imageY = drawY + (fontSize - imageHeight) / 2;

		//描画
		DrawExtendGraph(drawX, imageY, drawX + imageWidth, imageY + imageHeight, foundImage, true);

		//画像幅だけ右にずらす
		drawX += imageWidth;

		//描画済みの部分を削除
		remaining = remaining.substr(foundPos + foundKey.length());
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
	textImages_.push_back({ L"Lスティックを動かす",res.Load(ResourceManager::SRC::L_LEVER_MASHING).handleId_ });
	textImages_.push_back({ L"Lスティックレバガチャ",res.Load(ResourceManager::SRC::L_LEVER_MASHING).handleId_ });
}