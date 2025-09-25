#pragma once
#include "SceneBase.h"

class SceneGame : public SceneBase
{
public:

	//コンストラクタ
	SceneGame(void);
	//デストラクタ
	~SceneGame(void)override;

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

	//デバッグ描画
	void DebugDraw(void)override;
};

