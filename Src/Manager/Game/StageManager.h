#pragma once
#include<vector>
#include<memory>
#include<string>
#include"../Object/Stage/StageObject.h"

class StageManager
{
public:

	//オブジェクトの種類
	enum class OBJECT_TYPE
	{
		GLASS,		//草地
		BUILDING,	//ビル

		MAX,
	};

	//インスタンス生成
	static void CreateInstance(void);

	//インスタンス取得
	static StageManager& GetInstance(void) { return *instance_; }

	//読み込み
	void Load(void);

	//初期化
	void Init(void);

	//更新
	void Update(void);

	//描画
	void Draw(void);

	//解放
	void Destroy(void);

private:

	//名前
	std::unordered_map<OBJECT_TYPE, std::string> name_;

	//静的インスタンス
	static StageManager* instance_;

	//ステージ
    std::vector<std::unique_ptr<StageObject>> stages_;

	//コンストラクタ
	StageManager(void);

	//コピー禁止
	StageManager(const StageManager& _copy) = delete;
	StageManager& operator= (const StageManager& _copy) = delete;

	//デストラクタ
	~StageManager(void);
};