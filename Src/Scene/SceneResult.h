#pragma once
#include "SceneBase.h"
#include "../Object/Common/Transform.h"

class AnimationController;

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

	//プレイヤー座標
	static constexpr VECTOR PLAYER_POS = { -150.0f,-50.0f,130.0f };
	static constexpr VECTOR CAMERA_POS = { 0.0f,50.0f,-130.0f };

	//プレイヤーの相対座標
	static constexpr float PLAYER_LOCAL_POS = 100.0f;

	//順位座標
	static constexpr Vector2 RANK_POS = { 200,500 };
	static constexpr int RANK_LOCAL_POS = 200;

	//モデル情報
	std::vector<Transform> trans_;
	std::vector<std::unique_ptr<AnimationController>> anim_;

	//順位
	std::unordered_map<int, int> rankImg_;

	//背景
	int backImg_;
	int backCurtainImg_;
};

