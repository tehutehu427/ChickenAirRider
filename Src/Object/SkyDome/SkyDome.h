#pragma once
#include"../Common/Transform.h"

class SkyDome
{
public:

	//コンストラクタ
	SkyDome(void);
	
	//デストラクタ
	~SkyDome(void);

	//読み込み
	void Load(void);

	//初期化
	void Init(void);

	//更新
	void Update(void);

	//描画
	void Draw(const VECTOR& _pos);

private:

	//大きさ
	static constexpr float SCALE = 100.0f;

	//相対座標
	static constexpr VECTOR LOCAL_POS = { 0.0f,-0.0f,0.0f };

	//モデル
	int modelId_;

	//回転
	VECTOR rot_;

	//大きさ
	VECTOR scale_;
};

