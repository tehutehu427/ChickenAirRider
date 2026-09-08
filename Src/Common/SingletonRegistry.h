#pragma once
#include<vector>
#include<functional>

class SingletonRegistry
{
public:

	//削除タイミング
	enum class DESTROY_TIMING
	{
		GAME_END,	//ゲーム終了時
		ALL_END,	//ループ終了時
	};

	//コピー禁止
	SingletonRegistry(const SingletonRegistry& _copy) = delete;
	SingletonRegistry& operator=(const SingletonRegistry& _copy) = delete;

	//静的インスタンスの取得
	static SingletonRegistry& GetInstance(void);
	
	/// <summary>
	/// 破棄関数格納
	/// </summary>
	/// <param name="_timing">破棄タイミング</param>
	/// <param name="_func">破棄関数</param>
	void RegistryDestroyer(const DESTROY_TIMING _timing, const std::function<void(void)>& _func);

	//シングルトンの破棄
	void Delete(const DESTROY_TIMING _timing);

private:

	//シングルトンの破棄関数格納
	std::unordered_map<DESTROY_TIMING, std::vector<std::function<void(void)>>> destroyer_;

	//コンストラクタ
	SingletonRegistry(void);
	
	//デストラクタ
	~SingletonRegistry(void);
};

