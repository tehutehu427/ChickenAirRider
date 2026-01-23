#pragma once
#include "LastGameBase.h"

class DeathMatch : public LastGameBase
{
public:

	//コンストラクタ
	DeathMatch(SceneGame& _parent);
	
	//デストラクタ
	~DeathMatch(void)override;

	//初期化
	void Init(void)override;

	//初期化
	void Update(void)override;

	//初期化
	void Draw(const Camera& _camera)override;

	//初期化
	void Release(void)override;

};

