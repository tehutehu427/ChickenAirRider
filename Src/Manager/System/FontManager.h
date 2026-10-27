#pragma once
#include <string>
#include"../../Common/Singleton.h"
#include"FontType.h"

class FontManager : public Singleton<FontManager>
{
	//継承元のコンストラクタ等にアクセスするため
	friend class Singleton<FontManager>;

public:

	//フォント名の取得
	const std::wstring GetFontName(const FONT_TYPE& _type)const;

	/// <summary>
	/// フォントハンドルの取得
	/// </summary>
	/// <param name="_type">フォントの種類</param>
	/// <param name="_size">フォントサイズ</param>
	/// <param name="_thickness">フォントの厚さ</param>
	/// <returns></returns>
	const int GetFontHandle(const FONT_TYPE& _type, const int _size, const int _thick);

private:

	//登録用パス
	std::wstring fontName_[static_cast<int>(FONT_TYPE::MAX)];
	std::wstring fontPath_[static_cast<int>(FONT_TYPE::MAX)];

	//フォントのハンドルID
	std::array<std::map<std::pair<int,int>, int>, static_cast<int>(FONT_TYPE::MAX)> fontHandle_;

	//コンストラクタ
	FontManager(void);

	//デストラクタ
	~FontManager(void)override;

	//読み込み
	void LoadOutSide(void)override;

	//解放処理
	void Destroy(void)override;
};