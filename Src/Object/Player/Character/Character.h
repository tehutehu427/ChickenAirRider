#pragma once
#include"../../Common/Parameter.h"

class Character
{
public:

	//キャラクターの種類
	enum class CHARA_TYPE
	{
		NONE = -1,

		CHICKEN,
		
		MAX,
	};

	//コンストラクタ
	Character(void);

	//デストラクタ
	~Character(void);

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

