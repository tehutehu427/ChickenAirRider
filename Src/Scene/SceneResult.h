#pragma once
#include "SceneBase.h"
#include "../Object/Common/Transform.h"

class SceneResult : public SceneBase
{
public:

	//コンストラクタ
	SceneResult(void);
	//デストラクタ
	~SceneResult(void)override;

	//読み込み
	void Load(void)override;
	//初期化
	void Init(void)override;
	//更新
	void Update(void)override;
	//描画
	void Draw(const Camera& _camera)override;
	//解放
	void Release(void)override;

private:

	//モデル情報
	std::vector<Transform> trans_;

	//順位
	std::unordered_map<int, int> rankImg_;
};

