#pragma once
#include "../../Common/Singleton.h"
#include"FontType.h"

class TextManager : public Singleton<TextManager>
{
	//継承元のコンストラクタ等にアクセスするため
	friend class Singleton<TextManager>;

public:

	//初期化
	void Init(void);

	/// <summary>
	/// テキスト描画(対応している画像があるなら画像に置き換える)
	/// </summary>
	/// <param name="_str">文字列</param>
	/// <param name="_x">X座標</param>
	/// <param name="_y">Y座標</param>
	/// <param name="_size">フォントサイズ</param>
	/// <param name="_thick">フォントの厚さ</param>
	/// <param name="_color">フォントの色</param>
	/// <param name="_font">フォント</param>
	void DrawTextToImage(const std::wstring& _str, const int _x, const int _y, const int _size, const int _thick, const int _color, const FONT_TYPE _font = FONT_TYPE::DOT)const;

private:

	//テキストを画像変換する用
	struct TextImage
	{
		//変換する文字列
		std::wstring text = L"";

		//変換後の画像
		int image = -1;
	};

	//テキスト画像
	std::vector<TextImage> textImages_;

	//コンストラクタ
	TextManager(void);

	//デストラクタ
	~TextManager(void);

	//読み込み
	void LoadOutSide(void)override;
};