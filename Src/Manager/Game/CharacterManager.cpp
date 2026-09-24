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
	importData_ = LoaderManager<CharacterImportData>::GetInstance().GetfileData(Utility::WStrToStr(Application::PATH_OUTSIDE + L"Character.json"));
}

std::unique_ptr<Character> CharacterManager::GetCharacter(const std::string& _name) const
{
	//モデルIDの取得
	const int modelId = (this->*getModelId_[_name])();

	//キャラクターの生成
	for (auto& data : importData_)
	{
		//モデルIDの取得
		const int modelId = (this->*getModelId_[data.name])();

		std::unique_ptr<Character> character = std::make_unique<Character>();
		character->Load(data, modelId);
		characters_[data.name] = std::move(character);
	}

	return ;
}

CharacterManager::CharacterManager(void)
{
	//リソース
	getModelId_["chicken"] = &CharacterManager::GetChickenModelId;
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
