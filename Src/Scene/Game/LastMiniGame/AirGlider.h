#pragma once
#include "LastGameBase.h"

class AirGlider : public LastGameBase
{
public:

	//コンストラクタ
	AirGlider(SceneGame& _parent);

	//デストラクタ
	~AirGlider(void)override;

	//初期化
	void Init(void)override;

	//初期化
	void Update(void)override;

	//初期化
	void Draw(void)override;

	//初期化
	void Release(void)override;
};

