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
		BATTLE,		//バトルスター
		BOTTLE,		//ボトルスター

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
	void CreateMachine(const int _machineType ,const VECTOR& _pos);

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

	//機体の生成範囲
	static constexpr float CREATE_RANGE = 1000.0f;

	//デバッグの機体の生成相対位置
	static constexpr VECTOR DEBUG_CREATE_POS = { 0.0f,-200.0f,400.0f };
	static constexpr float DEBUG_CREATE_LOCAL_POS_X = 200.0f;

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

