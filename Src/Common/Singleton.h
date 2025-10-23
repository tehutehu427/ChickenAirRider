#pragma once
#include "SingletonRegistry.h"

template<typename T>
class Singleton
{
public:

	//コピー禁止
	Singleton(const Singleton& _copy) = delete;
	Singleton& operator=(const Singleton& _copy) = delete;

	//明示的にインスタンスの生成
	static void CreateInstance(void)
	{
		if (instance_ != nullptr)return;
		
		//生成
		instance_ = new T();

		//初期化
		instance_->Init();

		//レジスタに格納
		SingletonRegistry::GetInstance().RegistryDestroyer([](void) { instance_->Destroy(); });
	}

	//インスタンスの取得
	static T& GetInstance(void)
	{
		return *instance_;
	}

	//初期化
	virtual void Init(void) {}

	//インスタンスの破棄
	virtual void Destroy(void)
	{
		if (instance_ == nullptr)return;

		//破棄
		delete instance_;
		instance_ = nullptr;
	}

protected:

	//静的インスタンス
	static T* instance_;

	//コンストラクタ
	Singleton(void) = default;

	//デストラクタ
	virtual ~Singleton(void) = default;
};

//静的インスタンスの初期化
template<typename T>
T* Singleton<T>::instance_ = nullptr;