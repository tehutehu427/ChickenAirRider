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
	void Draw(const Camera& _camera)override;

	//初期化
	void Release(void)override;

private:

	//プレイヤーのスタート位置
	static constexpr VECTOR START_POS = { -100.0f,600.0f,0.0f };
};

