#pragma once
#include"../../Common/Parameter.h"

class Machine
{
public:

	enum class MACHINE_TYPE
	{
		NONE = -1,

		WAKABA,		//初期機体
		
		MAX
	};

	//コンストラクタ
	Machine(void);

	//デストラクタ
	~Machine(void);

	//読み込み
	void Load(void);

	//初期化
	void Init(void);

	//更新
	void Update(void);

	//描画
	void Draw(void);

	//パラメーターの取得
	const Parameter& GetParam(void) { return param_; }

private:

	//パラメーター
	Parameter param_;
};

