#pragma once
#include"SceneGame.h"

class SkyDome;

class GameBase
{
public:

	//コンストラクタ
	GameBase(SceneGame& _parent);
	
	//デストラクタ
	virtual ~GameBase(void);

	//初期化
	virtual void Init(void) = 0;

	//更新
	virtual void Update(void) = 0;

	//描画
	virtual void Draw(void) = 0;

	//解放
	virtual void Release(void) = 0;

protected:

	//デバッグ描画
	virtual void DebugDraw(void);

	//ゲームシーン
	SceneGame& parent_;

	//スカイドーム
	std::shared_ptr<SkyDome> sky_;
};

