#include "../pch.h"
#include"../Application.h"
#include"../Utility/Utility.h"
#include"../Loader/LoaderManager.h"
#include"../Manager/System/ResourceManager.h"
#include"../Object/Player/Machine/Machine.h"
#include "MachineManager.h"

MachineManager::MachineManager(void)
{
	//リソース
	getModelId_["wakaba"] = [this](void)
	{
		//インスタンス
		auto& res = ResourceManager::GetInstance();
		return res.LoadModelDuplicate(ResourceManager::SRC::WAKABA_STAR);
	};
	//リソース
	getModelId_["earth"] = [this](void)
	{
		//インスタンス
		auto& res = ResourceManager::GetInstance();
		return res.LoadModelDuplicate(ResourceManager::SRC::EARTH_STAR);
	};

	number_["wakaba"] = MACHINE_TYPE::WAKABA;
	number_["earth"] = MACHINE_TYPE::EARTH;
}

MachineManager::~MachineManager(void)
{
	//機体を明示的に解放
	machines_.clear();
}

void MachineManager::Init(void)
{
	//ステージ情報
	importData_ = LoaderManager<MachineImportData>::GetInstance().GetfileData(Utility::WStrToStr(Application::PATH_OUTSIDE + L"Machine.json"));

	//機体の生成
	CreateMachine();
}

void MachineManager::Update(void)
{
	for (const auto& machine : machines_)
	{
		if (machine == nullptr)continue;

		//更新
		machine->Update();

		//当たり判定の整理
		machine->Sweep();
	}

	//削除された判定を配列から破棄
	std::erase_if(machines_, [](const std::unique_ptr<Machine>& _machine){return _machine == nullptr;});
	std::erase_if(machines_, [](const std::unique_ptr<Machine>& _machine){return _machine->IsDead();});

}

void MachineManager::Draw(void)
{
	for (const auto& machine : machines_)
	{
		if (machine == nullptr)continue;

		//描画
		machine->Draw();
	}
}

void MachineManager::CreateMachine(void)
{
	//機体の個数
	int size = importData_.size();
	
	//ランダム
	int rand = Utility::GetRandomValue(0, size - 1);
	rand = 1;

	//機体の種類
	int modelId = getModelId_.at(importData_[rand].name)();
	VECTOR pos = { 0.0f,-200.0f,400.0f };

	//機体
	std::unique_ptr<Machine> machine = std::make_unique<Machine>(importData_[rand], modelId, pos);
	machine->Load();
	machine->Init();
	machine->CreateCol();

	//機体生成
	machines_.push_back(std::move(machine));
}

void MachineManager::SaveGetOffMachine(std::unique_ptr<Machine> _machine)
{
	//コライダ生成
	_machine->CreateCol();

	//機体格納
	machines_.push_back(std::move(_machine));
}

const int MachineManager::GetModelId(const MACHINE_TYPE _machineName)
{
	return getModelId_[importData_[static_cast<int>(_machineName)].name]();
}

const float MachineManager::GetRadius(const MACHINE_TYPE _machineName)
{
	return importData_[static_cast<int>(_machineName)].hitRadius;;
}

std::unique_ptr<Machine> MachineManager::GetMachine(const Machine& _machine)
{
	//配列内から同じものを探す
	for (auto& machine : machines_)
	{
		//ポインタを比較
		if (machine.get() == &_machine)
		{
			//同じのを渡す
			return std::move(machine);
		}
	}
}

std::unique_ptr<Machine> MachineManager::GetCreateMachine(const MACHINE_TYPE _machineName)const 
{
	//機体添え字
	int num = static_cast<int>(_machineName);
	std::string name = importData_[num].name;

	//機体の種類
	int modelId = getModelId_.at(name)();

	//機体
	std::unique_ptr<Machine> machine = std::make_unique<Machine>(importData_[num], modelId);

	//返す
	return std::move(machine);
}
