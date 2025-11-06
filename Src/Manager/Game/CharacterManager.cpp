#include "../pch.h"
#include "../Application.h"
#include "../Utility/Utility.h"
#include "../Loader/LoaderManager.h"
#include "../Object/Player/Character/CharaImportData.h"
#include "CharacterManager.h"

CharacterManager* CharacterManager::instance_ = nullptr;

void CharacterManager::CreateInstance(void)
{
	if (instance_ == nullptr)
	{
		instance_ = new CharacterManager();
	}
}

void CharacterManager::Init(void)
{

}

void CharacterManager::Destroy(void)
{
	delete instance_;
	instance_ = nullptr;
}

const std::unordered_map<std::string, int>& CharacterManager::GetAnimationData(const std::string _name)
{
	return anim_[_name];
}

CharacterManager::CharacterManager(void)
{
	//情報取得
	const auto& importData = LoaderManager<CharaImportData>::GetInstance().GetfileData(Utility::WStrToStr(Application::PATH_OUTSIDE + L"Character.json"));

	//アニメーション格納
	for (const auto& data : importData)
	{
		std::unordered_map<std::string, int> animNum;
		
		for (const auto& animation : data.animation)
		{
			animNum.emplace(animation.first,animation.second);
		}

		anim_.emplace(data.name, animNum);
	}
}

CharacterManager::~CharacterManager(void)
{
}