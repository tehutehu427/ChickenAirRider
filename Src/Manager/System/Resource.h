#pragma once
#include <string>
#include <vector>

class Resource
{

public:
	
	// リソースタイプ
	enum class TYPE
	{
		NONE,
		IMG,		//画像
		IMGS,		//分割画像
		MASK,		//マスク画像
		MODEL,		//モデル
		FONT,		//フォント
		EFFEKSEER,	//エフェクト(エフェクシア)
	};
	// コンストラクタ
	Resource(void);

	/// @brief コンストラクタ
	/// @param type リソースのタイプ
	/// @param path リソースまでのパス
	Resource(TYPE type, const std::wstring& path);
	
	/// @brief コンストラクタ(IMGS用)
	/// @param type リソースのタイプ
	/// @param path リソースまでのパス
	/// @param numX 横分割数
	/// @param numY 縦分割数
	/// @param sizeX 分割後画像の1つ分の横サイズ
	/// @param sizeY 分割後画像の1つ分の縦サイズ
	Resource(TYPE type, const std::wstring& path, int numX, int numY, int sizeX, int sizeY);

	// デストラクタ
	~Resource(void);

	// 読み込み
	void Load(void);

	// 解放
	void Release(void);

	// 複数画像ハンドルを別配列にコピー
	void CopyHandle(int* imgs);

	// リソースタイプ
	TYPE type_;

	// リソースの読み込み先
	std::wstring path_;

	// 画像とモデルのハンドルID
	int handleId_;

	// IMGS::LoadDivGraph用
	int* handleIds_;
	int numX_;
	int numY_;
	int sizeX_;
	int sizeY_;

	// モデル複製用
	std::vector<int> duplicateModelIds_;

};
