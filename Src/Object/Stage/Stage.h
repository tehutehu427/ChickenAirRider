#pragma once
#include "../ObjectBase.h"

class Stage : public ObjectBase
{
public:

	//コンストラクタ
	Stage(void);
	
	//デストラクタ
	~Stage(void)override;

	//読み込み
	void Load(void)override;

	//初期化
	void Init(void)override;

	//更新
	void Update(void)override;

	//描画
	void Draw(void)override;

	//当たり判定処理
	void OnHit(const std::weak_ptr<Collider> _hitCol)override;

private:

	//ステージの大きさ
	static constexpr VECTOR STAGE_SIZE = { 1000.0f,10.0f,1000.0f };
};

