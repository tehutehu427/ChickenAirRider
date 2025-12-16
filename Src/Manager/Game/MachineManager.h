#pragma once
#include<vector>
#include<memory>
#include"../Object/Player/Machine/Machine.h"
#include"../Object/Player/Machine/MachineImportData.h"
#include"../Common/Singleton.h"

class MachineManager : public Singleton<MachineManager>
{
	//継承元のコンストラクタ等にアクセスするため
	friend class Singleton<MachineManager>;

public:

	//更新
	void Update(void);

	//更新
	void Draw(void);

	//機体生成
	void CreateMachine(void);

	//降りた機体の保持
	void SaveGetOffMachine(std::unique_ptr<Machine> _machine);

	//機体のモデルIDを取得
	const int GetModelId(const std::string _machineName);

	//半径取得
	const float GetRadius(const std::string _machineName);

	//機体の取得
	std::unique_ptr<Machine> GetMachine(const Machine& _machine);

private:

	//インポートデータ
	std::vector<MachineImportData> importData_;

	//名前管理
	std::unordered_map<std::string, std::function<int(void)>> getModelId_;
	std::unordered_map<std::string, int> number_;

	//機体の格納
	std::vector<std::unique_ptr<Machine>> machines_;

	//コンストラクタ
	MachineManager(void);

	//デストラクタ
	~MachineManager(void)override;

	//初期化
	void Init(void)override;
};

