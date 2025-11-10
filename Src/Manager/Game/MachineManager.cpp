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
	modelId_["wakaba"] = [this](void)
	{
		//インスタンス
		auto& res = ResourceManager::GetInstance();
		return res.LoadModelDuplicate(ResourceManager::SRC::WAKABA);
	};
	number_["wakaba"] = 0;
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
}

void MachineManager::Update(void)
{
	for (const auto& machine : machines_)
	{
		if (machine == nullptr)continue;

		//更新
		machine->Update();
	}
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
	//機体の種類
	int modelId = modelId_[importData_[number_["wakaba"]].name]();
	float radius = importData_[number_["wakaba"]].hitRadius;

	//機体
	std::unique_ptr<Machine> machine = std::make_unique<Machine>(modelId, radius);
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

const int MachineManager::GetModelId(const std::string _machineName)
{
	return modelId_[importData_[number_[_machineName]].name]();
}

const float MachineManager::GetRadius(const std::string _machineName)
{
	return importData_[number_[_machineName]].hitRadius;;
}

std::unique_ptr<Machine> MachineManager::GetMachine(const Machine& _machine)
{
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
