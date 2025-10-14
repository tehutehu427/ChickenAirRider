#pragma once
#include <string>

class FontRegistry
{
public:

	enum class FONT_TYPE
	{
		DOT,
		BOKUTATI,
		HANAZOME,
		PENGS,
		LOGO,
		BANANA,
		MAX
	};

	//フォント種類
	static constexpr int FONT_TYPES = static_cast<int>(FONT_TYPE::MAX);

	//フォントネーム(cppで設定)
	static const std::string DOT;
	static const std::string BOKUTATI;
	static const std::string HANAZOME;
	static const std::string PENGS;
	static const std::string LOGO;
	static const std::string BANANA;

	//コンストラクタ
	FontRegistry();

	//デストラクタ
	~FontRegistry();

	//初期化
	void Init();

	//解放処理
	void Destroy();

private:

	//登録用パス
	std::wstring fontPath_[FONT_TYPES];
};

