#pragma once
#include "../GameBase.h"

class LastGameBase : public GameBase
{
public:

	//コンストラクタ
	LastGameBase(SceneGame& _parent);

	//デストラクタ
	virtual ~LastGameBase(void)override;

	//初期化
	virtual void Init(void)override;

	//更新
	virtual void Update(void)override;

	//描画
	virtual void Draw(void)override;

	//解放
	virtual void Release(void)override;

private:

	//デバッグ描画
	virtual void DebugDraw(void)override;

};

