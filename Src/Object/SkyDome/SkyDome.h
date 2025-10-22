#pragma once
#include"../Common/Transform.h"

class SkyDome
{
public:

	//コンストラクタ
	SkyDome(const VECTOR& _follow);
	
	//デストラクタ
	~SkyDome(void);

	//読み込み
	void Load(void);

	//初期化
	void Init(void);

	//更新
	void Update(void);

	//描画
	void Draw(void);

private:

	static constexpr VECTOR LOCAL_POS = { 0.0f,-2000.0f,0.0f };

	//追従座標
	const VECTOR& follow_;

	//モデル
	int modelId_;

	//回転
	VECTOR rot_;

	//大きさ
	VECTOR scale_;
};

