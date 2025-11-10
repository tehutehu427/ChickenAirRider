#pragma once
#include "../Common/Singleton.h"

class AnimationManager : public Singleton<AnimationManager>
{
	//継承元のコンストラクタ等にアクセスするため
	friend class Singleton<AnimationManager>;

public:

	//読み込み
	void Init(void);

	//解放
	void Destroy(void);

	/// @brief アニメーションの番号を取得
	/// @param _name キャラクター名
	/// @return キャラクターのアニメーション番号
	const std::unordered_map<std::string, int>& GetAnimationData(const std::string _name);

private:

	//アニメーション名に
	std::unordered_map<std::string, std::unordered_map<std::string, int>> anim_;

	//コンストラクタ
	AnimationManager(void);

	//デストラクタ
	~AnimationManager(void)override;
};

