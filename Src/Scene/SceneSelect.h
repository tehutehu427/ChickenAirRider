#pragma once
#include "SceneBase.h"

class SceneSelect : public SceneBase
{
public:

	//コンストラクタ
	SceneSelect(void);
	//デストラクタ
	~SceneSelect(void)override;

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

