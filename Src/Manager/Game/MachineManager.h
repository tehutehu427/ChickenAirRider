#pragma once
#include<vector>
#include<memory>
#include<functional>
#include"../Object/Player/Machine/Machine.h"
#include"../Object/Player/Machine/MachineImportData.h"
#include"../Common/Singleton.h"

class MachineManager : public Singleton<MachineManager>
{
	//継承元のコンストラクタ等にアクセスするため
	friend class Singleton<MachineManager>;

public:

	//機体の種類
	enum class MACHINE_TYPE
	{
		NONE = -1,

		WAKABA,		//ワカバスター
		EARTH,		//アーススター

		MAX
	};

	//初期化
	void Init(void)override;

	//更新
	void Update(void);

	//更新
	void Draw(void);

	//機体生成
	void CreateMachine(void);

	//降りた機体の保持
	void SaveGetOffMachine(std::unique_ptr<Machine> _machine);

	//機体のモデルIDを取得
	const int GetModelId(const MACHINE_TYPE _machineName);

	//半径取得
	const float GetRadius(const MACHINE_TYPE _machineName);

	//機体の取得
	std::unique_ptr<Machine> GetMachine(const Machine& _machine);

	//機体を生成して返す
	std::unique_ptr<Machine> GetCreateMachine(const MACHINE_TYPE _machineName)const;

private:

	//インポートデータ
	std::vector<MachineImportData> importData_;

	//名前管理
	std::unordered_map<std::string, std::function<int(void)>> getModelId_;
	std::unordered_map<std::string, MACHINE_TYPE> number_;

	//機体の格納
	std::vector<std::unique_ptr<Machine>> machines_;

	//コンストラクタ
	MachineManager(void);

	//デストラクタ
	~MachineManager(void)override;

	//読み込み
	void LoadOutSide(void)override;
};

