#pragma once
#include<vector>
#include<memory>
#include<string>

class Stage;

class StageManager
{
public:

	//インスタンス生成
	static void CreateInstance(void);

	//インスタンス取得
	static StageManager& GetInstance(void) { return *instance_; }

	//初期化
	void Init(void);

	//更新
	void Update(void);

	//描画
	void Draw(void);

	//解放
	void Destroy(void);

	//Jsonのロード
	void LoadJsonFile(const std::string& _fileName);

private:

	//静的インスタンス
	static StageManager* instance_;

	//ステージ
	std::unique_ptr<Stage> stage_;

	//コンストラクタ
	StageManager(void);

	//コピー禁止
	StageManager(const StageManager& _copy) = delete;
	StageManager& operator= (const StageManager& _copy) = delete;

	//デストラクタ
	~StageManager(void);
};

