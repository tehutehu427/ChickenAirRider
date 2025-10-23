#pragma once
#include<vector>
#include<functional>

class SingletonRegistry
{
public:

	//コピー禁止
	SingletonRegistry(const SingletonRegistry& _copy) = delete;
	SingletonRegistry& operator=(const SingletonRegistry& _copy) = delete;

	//静的インスタンスの取得
	static SingletonRegistry& GetInstance(void)
	{
		static SingletonRegistry instance;
		return instance;
	}

	/// @brief 破棄関数格納
	/// @param _func 破棄関数
	void RegistryDestroyer(const std::function<void(void)>& _func)
	{
		destroyer_.push_back(_func);
	}

	//シングルトンの破棄
	void AllDelete(void)
	{
		// 登録の逆順で破棄（依存関係を考慮）
		for (auto it = destroyer_.rbegin(); it != destroyer_.rend(); ++it) {
			(*it)();
		}

		//配列初期化
		destroyer_.clear();
	}

private:

	//シングルトンの破棄関数格納
	std::vector<std::function<void(void)>> destroyer_;

	//コンストラクタ
	SingletonRegistry(void) = default;
	
	//デストラクタ
	~SingletonRegistry(void) = default;
};

