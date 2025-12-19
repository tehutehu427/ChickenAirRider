#pragma once
#include "GameBase.h"

class GameCheck : public GameBase
{
public:

	//コンストラクタ
	GameCheck(SceneGame& _parent);

	//デストラクタ
	~GameCheck(void)override;

	//初期化
	void Init(void)override;

	//更新
	void Update(void)override;

	//描画
	void Draw(void)override;

	//解放
	void Release(void)override;

private:

	//デバッグ描画
	void DebugDraw(void)override;

};

