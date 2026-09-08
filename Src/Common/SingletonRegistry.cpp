#include "../pch.h"
#include "SingletonRegistry.h"

SingletonRegistry& SingletonRegistry::GetInstance(void)
{
	static SingletonRegistry instance;
	return instance;
}

void SingletonRegistry::RegistryDestroyer(const DESTROY_TIMING _timing, const std::function<void(void)>& _func)
{
	destroyer_[_timing].push_back(_func);
}

void SingletonRegistry::Delete(const DESTROY_TIMING _timing)
{
	// 登録の逆順で破棄（依存関係を考慮）
	for (auto it = destroyer_[_timing].rbegin(); it != destroyer_[_timing].rend(); ++it) {
		if (*it == nullptr) continue;
		(*it)();
	}

	//配列初期化
	destroyer_[_timing].clear();
}

SingletonRegistry::SingletonRegistry(void)
{
}

SingletonRegistry::~SingletonRegistry(void)
{
}
