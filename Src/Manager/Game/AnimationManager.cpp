#include "../pch.h"
#include "../Application.h"
#include "../Utility/Utility.h"
#include "../Loader/LoaderManager.h"
#include "../Object/Player/AnimationImportData.h"
#include "AnimationManager.h"

void AnimationManager::Init(void)
{

}

void AnimationManager::Destroy(void)
{
	delete instance_;
	instance_ = nullptr;
}

const std::unordered_map<std::string, int>& AnimationManager::GetAnimationData(const std::string _name)
{
	return anim_[_name];
}

AnimationManager::AnimationManager(void)
{
	//情報取得
	const auto& importData = LoaderManager<AnimationImportData>::GetInstance().GetfileData(Utility::WStrToStr(Application::PATH_OUTSIDE + L"Animation.json"));

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

AnimationManager::~AnimationManager(void)
{
}