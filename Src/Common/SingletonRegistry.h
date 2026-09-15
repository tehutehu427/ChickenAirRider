#pragma once
#include<vector>
#include<unordered_map>
#include<functional>

class SingletonRegistry
{
public:

	//削除タイミング
	enum class DESTROY_TIMING
	{
		GAME_END,	//ゲーム終了時
		ALL_END,	//ループ終了時
		MAX
	};

	//コピー禁止
	SingletonRegistry(const SingletonRegistry& _copy) = delete;
	SingletonRegistry& operator=(const SingletonRegistry& _copy) = delete;

	//静的インスタンスの取得
	static SingletonRegistry& GetInstance(void)
	{
		if (instance_ == nullptr)
		{
			instance_ = new SingletonRegistry();
		}

		return *instance_;
	}
	
	//破棄用関数ポインタ
	using DestroyFunc = void (*)(void);

	/// <summary>
	/// 破棄関数格納
	/// </summary>
	/// <param name="_timing">破棄タイミング</param>
	/// <param name="_func">破棄関数</param>
	void RegistryDestroyer(const DESTROY_TIMING _timing, const DestroyFunc _func)
	{
		destroyer_[_timing].push_back(_func);
	}

	//シングルトンの破棄
	void Delete(const DESTROY_TIMING _timing)
	{
		//破棄関数の取得
		auto it = destroyer_.find(_timing);

		//破棄関数がない場合は何もしない
		if (it == destroyer_.end())
			return;

		//破棄関数の実行
		for (auto func = it->second.rbegin();
			func != it->second.rend();
			++func)
		{
			//関数が空なら何もしない
			if (*func == nullptr)
				continue;

			//関数の実行
			(*func)();
		}

		//破棄関数の削除
		destroyer_.erase(it);
	}

	//すべてのシングルトンの破棄
	void DeleteAll(void)
	{
		for (int i = 0 ; i < static_cast<int>(DESTROY_TIMING::MAX); i++)
		{
			Delete(static_cast<DESTROY_TIMING>(i));
		}
	}

	//シングルトンレジストリ自身の破棄(明示的な解放)
	void DeleteSelfInstance(void)
	{
		if (instance_ == nullptr)return;
	
		//すべてのシングルトンの破棄
		DeleteAll();

		//自身の破棄
		delete instance_;
		instance_ = nullptr;
	}

private:

	//静的インスタンス
	inline static SingletonRegistry* instance_ = nullptr;

	//シングルトンの破棄関数格納
	std::unordered_map<DESTROY_TIMING, std::vector<DestroyFunc>> destroyer_;

	//コンストラクタ
	SingletonRegistry(void) = default;
	
	//デストラクタ
	~SingletonRegistry(void) = default;
};

