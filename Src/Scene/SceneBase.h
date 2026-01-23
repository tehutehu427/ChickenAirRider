#pragma once
#include"../Manager/System/Camera.h"

class SceneBase
{
public:

	//コンストラクタ
	SceneBase(void);
	//デストラクタ
	virtual ~SceneBase(void);

	//読み込み
	virtual void Load(void) = 0;
	//初期化
	virtual void Init(void) = 0;
	//更新
	virtual void Update(void) = 0;
	//描画
	virtual void Draw(const Camera& _camera) = 0;
	//解放
	virtual void Release(void) = 0;

protected:

	//デバッグ描画
	virtual void DebugDraw(void);
};

