#pragma once
#include "SceneBase.h"

class SceneTitle : public SceneBase
{
public:

	//コンストラクタ
	SceneTitle(void);
	//デストラクタ
	~SceneTitle(void)override;

	//読み込み
	void Load(void)override;
	//初期化
	void Init(void)override;
	//更新
	void Update(void)override;
	//描画
	void Draw(void)override;
	//解放
	void Release(void)override;

private:

	int logoImg_;

	//デバッグ描画
	void DebugDraw(void)override;
};

