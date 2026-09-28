#include "../../pch.h"
#include "../../Application.h"
#include "../../Utility/Utility.h"
#include "../System/ResourceManager.h"
#include "../Loader/LoaderManager.h"
#include "../../Object/Player/Character/CharacterImportData.h"
#include "../../Object/Player/Character/Character.h"
#include "CharacterManager.h"

void CharacterManager::LoadOutSide(void)
{
	//情報取得
	importData_ = LoaderManager<CharacterImportData>::GetInstance().GetfileData(Utility::WStrToStr(Application::PATH_JSON + L"Character.json"));
}

std::unique_ptr<Character> CharacterManager::CreateCharacter(const std::string& _name)
{
	//名前が存在するか
	if (getModelId_.find(_name) == getModelId_.end())
	{
		return nullptr;
	}

	//モデルIDの取得
	auto func = getModelId_[_name];
	const int modelId = (this->*func)();

	//キャラクター作成
	std::unique_ptr<Character> character = std::make_unique<Character>();
	character->Load(importData_[static_cast<int>(charaType_[_name])], modelId);

	//作成したキャラクターを返す
	return character;
}

CharacterManager::CharacterManager(void)
{
	//リソース
	getModelId_["chicken"] = &CharacterManager::GetChickenModelId;

	//キャラクターの種類
	charaType_["chicken"] = CHARA_TYPE::CHICKEN;
}

CharacterManager::~CharacterManager(void)
{
}

const int CharacterManager::GetChickenModelId(void)
{
	//インスタンス
	auto& res = ResourceManager::GetInstance();
	return res.LoadModelDuplicate(ResourceManager::SRC::CHICKEN);
}
