#pragma once
#include<vector>
#include<memory>
#include<string>
#include<functional>
#include"../Object/Common/Collider.h"
#include"../Object/Stage/StageImportData.h"
#include"../System/Camera.h"
#include"../../Common/Singleton.h"

class Geometry;
class StageObject;

class StageManager : public Singleton<StageManager>
{
	//継承元のコンストラクタ等にアクセスするため
	friend class Singleton<StageManager>;

public:

	//ゲームモードごとのステージ
	enum class MODE
	{
		MAIN,			//基本
		BATTLE,			//バトル
		AIR_GLIDER,		//エアグライダー
		MAX
	};

	//読み込み
	void LoadOutSide(void)override;

	//初期化
	void Init(void)override;
	void Init(const MODE _mode, int _createNum = 1);

	//更新
	void Update(void);

	//描画
	void Draw(const Camera& _camera);

	//解放
	void Destroy(void);

	//モードの設定
	void SetMode(const MODE _mode) { mode_ = _mode; }

private:

	//現在のモード
	MODE mode_;

	//インポートデータ
	std::unordered_map<MODE,std::vector<StageImportData>> importData_;
	std::vector<StageImportData>gliderStartStageData_;

	//名前管理
	std::unordered_map<std::string, std::function<int(void)>> modelId_;
	std::unordered_map<std::string, Collider::TAG> tag_;

	//ステージ
    std::vector<std::unique_ptr<StageObject>> stages_;

	//更新
	std::unordered_map<MODE, std::function<void(void)>> update_;

	//コンストラクタ
	StageManager(void);

	//デストラクタ
	~StageManager(void)override;

	//更新
	void UpdateNormal(void);
	void UpdateLoop(void);
};