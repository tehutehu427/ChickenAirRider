#pragma once

class PlayerUIBase
{
public:

	//コンストラクタ
	PlayerUIBase(void) = default;

	//デストラクタ
	virtual ~PlayerUIBase(void) = default;

	//読み込み
	virtual void Load(void) = 0;

	//初期化
	virtual void Init(void) = 0;

	//更新
	virtual void Update(void) = 0;

	//描画
	virtual void Draw(void) = 0;

	//解放
	virtual void Release(void) = 0;

private:
};

